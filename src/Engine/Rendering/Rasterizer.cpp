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
#include "Shaders.hpp"

#include <algorithm>
#include <cstdint>

namespace Engine
{
	namespace Rendering
	{
		using namespace Display;
		using namespace Math;

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

		void Rasterizer::SetLightingSystem(std::shared_ptr<LightingSystem> lighting_system)
		{
			this->lighting_system = std::move(lighting_system);
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

		void Rasterizer::DrawTriangle(DepthBuffer& depthbuffer, const Vertex& v0, const Vertex& v1, const Vertex& v2, const HSVColor& color)
		{
			DrawClippedTriangle(depthbuffer, v0, v1, v2, color, Shaders::fixed_color_shader);
		}

		void Rasterizer::DrawShadedTriangle(DepthBuffer& depthbuffer, const Vertex& v0, const Vertex& v1, const Vertex& v2, const HSVColor& color)
		{
			DrawClippedTriangle(depthbuffer, v0, v1, v2, color, Shaders::shaded_color_shader);
		}

		void Rasterizer::DrawClippedTriangle(DepthBuffer& depthbuffer, Vertex v0, Vertex v1, Vertex v2, const HSVColor& color, std::function<void(SHADER_ARGUMENTS)> shader)
		{
			TransformVertexModel(v0);
			TransformVertexModel(v1);
			TransformVertexModel(v2);

			Vertex original_v0 = v0;
			Vertex original_v1 = v1;
			Vertex original_v2 = v2;

			TransformVertexViewProjection(v0);
			TransformVertexViewProjection(v1);
			TransformVertexViewProjection(v2);

			if (IsBackface(v0.GetPosition(), v1.GetPosition(), v2.GetPosition()))
				return;

			// if all three position components are inside the view frustum it doesn't need to be clipped
			if (v0.IsInsideViewFrustum() && v1.IsInsideViewFrustum() && v2.IsInsideViewFrustum())
			{
				TransformVertexScreenspace(v0);
				TransformVertexScreenspace(v1);
				TransformVertexScreenspace(v2);

				Triangle triangle =
				{
					v0.GetPosition(),
					v1.GetPosition(),
					v2.GetPosition(),
					original_v0,
					original_v1,
					original_v2
				};

				RasterizeTriangle(depthbuffer, triangle, color, shader);
				return;
			}

			// store the verts in a buffer to clip them
			std::array<Vertex, 10> vertices_buffer = { v0, v1, v2 };
			uint8_t vertices_buffer_count = 3;

			bool clip_x_drawable = clipper.ClipVerticesAgainstAxis(vertices_buffer, &vertices_buffer_count, ClipAxis::AXIS_X);
			bool clip_y_drawable = clipper.ClipVerticesAgainstAxis(vertices_buffer, &vertices_buffer_count, ClipAxis::AXIS_Y);
			bool clip_z_drawable = clipper.ClipVerticesAgainstAxis(vertices_buffer, &vertices_buffer_count, ClipAxis::AXIS_Z);

			if (!clip_x_drawable || !clip_y_drawable || !clip_z_drawable)
				return;

			// draw the polygon as a triangle fan
			for (uint8_t i = 1; i < vertices_buffer_count - 1; i++)
			{
				Vertex clipped_v0 = vertices_buffer[0];
				Vertex clipped_v1 = vertices_buffer[i];
				Vertex clipped_v2 = vertices_buffer[i + 1];

				Vertex clipped_original_v0 = GetTransformedVertexInverseViewProjection(clipped_v0);
				Vertex clipped_original_v1 = GetTransformedVertexInverseViewProjection(clipped_v1);
				Vertex clipped_original_v2 = GetTransformedVertexInverseViewProjection(clipped_v2);

				TransformVertexScreenspace(clipped_v0);
				TransformVertexScreenspace(clipped_v1);
				TransformVertexScreenspace(clipped_v2);

				Triangle triangle =
				{
					clipped_v0.GetPosition(),
					clipped_v1.GetPosition(),
					clipped_v2.GetPosition(),
					clipped_original_v0,
					clipped_original_v1,
					clipped_original_v2
				};

				RasterizeTriangle(depthbuffer, triangle, color, shader);
			}
		}


		void Rasterizer::RasterizeTriangle(DepthBuffer& depthbuffer, const Triangle& triangle, const HSVColor& color, std::function<void(SHADER_ARGUMENTS)> shader)
		{
			uint16_t bb_min_x = std::min({ (uint16_t)triangle.screen_p0.x, (uint16_t)triangle.screen_p1.x, (uint16_t)triangle.screen_p2.x });
			uint16_t bb_min_y = std::min({ (uint16_t)triangle.screen_p0.y, (uint16_t)triangle.screen_p1.y, (uint16_t)triangle.screen_p2.y });
			uint16_t bb_max_x = std::max({ (uint16_t)triangle.screen_p0.x, (uint16_t)triangle.screen_p1.x, (uint16_t)triangle.screen_p2.x });
			uint16_t bb_max_y = std::max({ (uint16_t)triangle.screen_p0.y, (uint16_t)triangle.screen_p1.y, (uint16_t)triangle.screen_p2.y });

			Point2 point = Point2(bb_min_x, bb_min_y);

			TriangleEdge edge0 = TriangleEdge(triangle.screen_p1, triangle.screen_p2, point);
			TriangleEdge edge1 = TriangleEdge(triangle.screen_p2, triangle.screen_p0, point);
			TriangleEdge edge2 = TriangleEdge(triangle.screen_p0, triangle.screen_p1, point);
			TriangleEdge area = TriangleEdge(triangle.screen_p0, triangle.screen_p1, triangle.screen_p2);

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
						float barcoord2 = edge2_mag_xy / (float)area.edgefunction_res;

						float z = barcoord0 * triangle.screen_p0.z + barcoord1 * triangle.screen_p1.z + barcoord2 * triangle.screen_p2.z;
					
						if (depthbuffer.GetDepth(x, y) > z)
						{
							shader(x, y, z, lighting_system, renderer, barcoord0, barcoord1, barcoord2, triangle.v0, triangle.v1, triangle.v2, color);
							depthbuffer.SetDepth(x, y, z);
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