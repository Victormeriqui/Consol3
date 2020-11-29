#include "Rasterizer.hpp"

#include "../../Display/IRenderer.hpp"
#include "../../Math/Point2.hpp"
#include "../../Math/Matrix4.hpp"
#include "Vertex.hpp"
#include "../../Display/RGBColor.hpp"
#include "../../Display/HSVColor.hpp"
#include "Transform.hpp"
#include "../../Math/Util/MathUtil.hpp"
#include "Clipper.hpp"
#include "DepthBuffer.hpp"
#include "Shaders/IShader.hpp"
#include "VertexBuffer.hpp"
#include "Triangle.hpp"

#include <algorithm>
#include <cstdint>

namespace Engine
{
	namespace Rendering
	{
		using namespace Display;
		using namespace Math;
		using namespace Shaders;

		Rasterizer::Rasterizer(std::shared_ptr<IRenderer> renderer) :
			renderer(renderer),
			clipper(Clipper())
		{
			float width_h = renderer->GetFrameBufferWidth() / 2.0f;
			float height_h = renderer->GetFrameBufferHeight() / 2.0f;
			float mat[4][4];

			mat[0][0] = width_h; mat[0][1] = 0;         mat[0][2] = 0; mat[0][3] = width_h;
			mat[1][0] = 0;       mat[1][1] = -height_h; mat[1][2] = 0; mat[1][3] = height_h;
			mat[2][0] = 0;       mat[2][1] = 0;         mat[2][2] = 1; mat[2][3] = 0;
			mat[3][0] = 0;       mat[3][1] = 0;         mat[3][2] = 0; mat[3][3] = 1;

			viewport_mat = Matrix4(mat);
		}

		Vertex& Rasterizer::TransformVertexModel(Vertex& vertex)
		{
			vertex *= model_mat;
			vertex.TransformNormals(normal_mat);

			return vertex;
		}

		Vertex& Rasterizer::TransformVertexViewProjection(Vertex& vertex)
		{
			vertex *= view_mat;
			vertex *= projection_mat;

			return vertex;
		}

		Vertex Rasterizer::GetTransformedVertexInverseViewProjection(const Vertex& vertex)
		{
			return (Vertex(vertex) * inverse_projection_mat) * inverse_view_mat;
		}

		Vertex& Rasterizer::TransformVertexScreenspace(Vertex& vertex)
		{
			vertex *= viewport_mat;
			vertex.PerspectiveDivide();
			return vertex;
		}

		bool Rasterizer::IsBackface(const Vector3& p0, const Vector3& p1, const Vector3& p2) const
		{
			Vector3 center = Vector3(
				(p0.x + p1.x + p2.x) / 3,
				(p0.y + p1.y + p2.y) / 3,
				(p0.z + p1.z + p2.z) / 3);

			Vector3 edge1 = p0 - p1;
			Vector3 edge2 = p1 - p2;

			Vector3 facenormal = edge1.GetCrossProduct(edge2);

			return center.GetDotProduct(facenormal) > 0;
		}

		TriangleEdge::TriangleEdge(const Point2& v_a, const Point2& v_b, const Point2& start_point)
		{
			// edge function: F(p) = (v_a.y - v_b.y)*start_point.x + (v_b.x - v_a.x)*start_point.y + (v_a.x * v_b.y - v_a.y * v_a.x)
			// this will have one sign if the point is to the left of the edge (v_a -> v_b), and another sign if it is to the right
			// the actual sign will depend if the edges are defined clockwise or counter-clockwise

			// calculate each component without the point
			int32_t comp1 = v_a.y - v_b.y;
			int32_t comp2 = v_b.x - v_a.x;
			int32_t comp3 = v_a.x * v_b.y - v_a.y * v_b.x;

			// step deltas
			step_delta_x = comp1;
			step_delta_y = comp2;

			// calculate the whole edge function for the point
			edgefunction_res = (comp1 * start_point.x) + (comp2 * start_point.y) + comp3;
		}

		void Rasterizer::DrawVertexBuffer(DepthBuffer& depthbuffer, const VertexBuffer& vertex_buffer, const HSVColor& color, const IShader& frag_shader)
		{
			ClipAndRasterize(depthbuffer, vertex_buffer, color, frag_shader);
		}

		void Rasterizer::ClipAndRasterize(DepthBuffer& depthbuffer, const VertexBuffer& vertex_buffer, const HSVColor& color, const IShader& frag_shader)
		{
			for (uint32_t i = 0; i < vertex_buffer.GetIndices().size(); i += 3)
			{
				Vertex v0 = vertex_buffer.GetVertex(i);
				Vertex v1 = vertex_buffer.GetVertex(i + 1);
				Vertex v2 = vertex_buffer.GetVertex(i + 2);

				TransformVertexModel(v0);
				TransformVertexModel(v1);
				TransformVertexModel(v2);

				// store original, model transformed vertices
				Vertex model_v0 = v0;
				Vertex model_v1 = v1;
				Vertex model_v2 = v2;

				TransformVertexViewProjection(v0);
				TransformVertexViewProjection(v1);
				TransformVertexViewProjection(v2);

				if (IsBackface(v0.GetPosition(), v1.GetPosition(), v2.GetPosition()))
					continue;

				// if all three position components are inside the view frustum it doesn't need to be clipped
				if (v0.IsInsideViewFrustum() && v1.IsInsideViewFrustum() && v2.IsInsideViewFrustum())
				{
	
					TransformVertexScreenspace(v0);
					TransformVertexScreenspace(v1);
					TransformVertexScreenspace(v2);

					Triangle triangle
					{
						model_v0,
						model_v1,
						model_v2,
						// store view projected W for perspective correct interpolation
						1.0f / v0.GetW(),
						1.0f / v1.GetW(),
						1.0f / v2.GetW(),
						v0.GetPosition(),
						v1.GetPosition(),
						v2.GetPosition(),
					};

					RasterizeTriangle(depthbuffer, triangle, color, frag_shader);
					
					continue;
				}

				// store the verts in a buffer to clip them
				std::array<Vertex, 10> vertices_buffer = { v0, v1, v2 };
				uint8_t vertices_buffer_count = 3;

				bool clip_x_drawable = clipper.ClipVerticesAgainstAxis(vertices_buffer, &vertices_buffer_count, ClipAxis::AXIS_X);
				bool clip_y_drawable = clipper.ClipVerticesAgainstAxis(vertices_buffer, &vertices_buffer_count, ClipAxis::AXIS_Y);
				bool clip_z_drawable = clipper.ClipVerticesAgainstAxis(vertices_buffer, &vertices_buffer_count, ClipAxis::AXIS_Z);

				if (!clip_x_drawable || !clip_y_drawable || !clip_z_drawable)
					continue;

				// draw the polygon as a triangle fan
				for (uint8_t i = 1; i < vertices_buffer_count - 1; i++)
				{
					Vertex clipped_v0 = vertices_buffer[0];
					Vertex clipped_v1 = vertices_buffer[i];
					Vertex clipped_v2 = vertices_buffer[i + 1];

					TransformVertexScreenspace(clipped_v0);
					TransformVertexScreenspace(clipped_v1);
					TransformVertexScreenspace(clipped_v2);

					Triangle triangle
					{
						// store original, model transformed vertices
						GetTransformedVertexInverseViewProjection(clipped_v0),
						GetTransformedVertexInverseViewProjection(clipped_v1),
						GetTransformedVertexInverseViewProjection(clipped_v2),
						// store view projected W for perspective correct interpolation
						1.0f / clipped_v0.GetW(),
						1.0f / clipped_v1.GetW(),
						1.0f / clipped_v2.GetW(),
						clipped_v0.GetPosition(),
						clipped_v1.GetPosition(),
						clipped_v2.GetPosition()
					};

					RasterizeTriangle(depthbuffer, triangle, color, frag_shader);
				}
			}
		}

		void Rasterizer::RasterizeTriangle(DepthBuffer& depthbuffer, const Triangle& triangle, const HSVColor& color, const IShader& frag_shader)
		{
			uint16_t bb_min_x = std::min({ (uint16_t)triangle.v0_screen.x, (uint16_t)triangle.v1_screen.x, (uint16_t)triangle.v2_screen.x });
			uint16_t bb_min_y = std::min({ (uint16_t)triangle.v0_screen.y, (uint16_t)triangle.v1_screen.y, (uint16_t)triangle.v2_screen.y });
			uint16_t bb_max_x = std::max({ (uint16_t)triangle.v0_screen.x, (uint16_t)triangle.v1_screen.x, (uint16_t)triangle.v2_screen.x });
			uint16_t bb_max_y = std::max({ (uint16_t)triangle.v0_screen.y, (uint16_t)triangle.v1_screen.y, (uint16_t)triangle.v2_screen.y });

			Point2 point = Point2(bb_min_x, bb_min_y);

			TriangleEdge edge0 = TriangleEdge(triangle.v1_screen, triangle.v2_screen, point);
			TriangleEdge edge1 = TriangleEdge(triangle.v2_screen, triangle.v0_screen, point);
			TriangleEdge edge2 = TriangleEdge(triangle.v0_screen, triangle.v1_screen, point);
			TriangleEdge area = TriangleEdge(triangle.v0_screen, triangle.v1_screen, triangle.v2_screen);

			// degenerate triangle
			if (area.edgefunction_res == 0)
				return;

			for (uint16_t y = bb_min_y; y <= bb_max_y; y++)
			{
				int32_t edge0_mag_xy = edge0.edgefunction_res;
				int32_t edge1_mag_xy = edge1.edgefunction_res;
				int32_t edge2_mag_xy = edge2.edgefunction_res;

				for (uint16_t x = bb_min_x; x <= bb_max_x; x++)
				{
					if ((edge0_mag_xy | edge1_mag_xy | edge2_mag_xy) >= 0)
					{
						float barcoord0 = edge0_mag_xy / (float)area.edgefunction_res;
						float barcoord1 = edge1_mag_xy / (float)area.edgefunction_res;
						// the sum of the 3 barycentric coords is 1, this avoids a division
						float barcoord2 = 1.0f - (barcoord0 + barcoord1);

						float z = barcoord0 * triangle.v0_screen.z + 
								barcoord1 * triangle.v1_screen.z + 
								barcoord2 * triangle.v2_screen.z;
					
						if (depthbuffer.GetDepth(x, y) > z)
						{
							HSVColor out_color = color;
							frag_shader.FragmentShader(out_color, triangle, barcoord0, barcoord1, barcoord2);
							depthbuffer.SetDepth(x, y, z);
							renderer->SetPixel(x, y, out_color);
						}
					}

					edge0_mag_xy += edge0.step_delta_x;
					edge1_mag_xy += edge1.step_delta_x;
					edge2_mag_xy += edge2.step_delta_x;
				}

				edge0.edgefunction_res += edge0.step_delta_y;
				edge1.edgefunction_res += edge1.step_delta_y;
				edge2.edgefunction_res += edge2.step_delta_y;
			}
		}

		void Rasterizer::SetModelMatrix(const Transform& model_transform)
		{
			model_mat = model_transform.GetTranslationMatrix() * (model_transform.GetRotationMatrix() * model_transform.GetScaleMatrix());
			// shouldn't this also have the view_mat?
			// TODO: figure out why this works
			normal_mat = Matrix4(model_mat).Invert().Transpose();
		}

		void Rasterizer::SetModelMatrix(const Matrix4& model_matrix)
		{
			model_mat = model_matrix;
			normal_mat = Matrix4(model_mat).Invert().Transpose();
		}

		void Rasterizer::SetViewMatrix(const Matrix4& view_matrix)
		{
			view_mat = view_matrix;
			inverse_view_mat = view_mat.GetInverted();
		}

		void Rasterizer::SetProjectionMatrix(const Matrix4& projection_matrix)
		{
			projection_mat = projection_matrix;
			inverse_projection_mat = projection_mat.GetInverted();
		}

		void Rasterizer::SetViewportMatrix(const Matrix4& viewport_matrix)

		{
			viewport_mat = viewport_matrix;
		}
	}
}