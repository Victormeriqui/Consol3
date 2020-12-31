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

		void Rasterizer::DrawVertexBuffer(DepthBuffer& depthbuffer, const VertexBuffer& vertex_buffer, const HSVColor& color, IShader& shader)
		{
			ClipAndRasterize(depthbuffer, vertex_buffer, color, shader);
		}

		void Rasterizer::ClipAndRasterize(DepthBuffer& depthbuffer, const VertexBuffer& vertex_buffer, const HSVColor& color, IShader& shader)
		{
			for (uint32_t i = 0; i < vertex_buffer.GetIndices().size(); i += 3)
			{
				Vertex v0 = vertex_buffer.GetVertex(i);
				Vertex v1 = vertex_buffer.GetVertex(i + 1);
				Vertex v2 = vertex_buffer.GetVertex(i + 2);

				MVPTransform vs_shader_mats =
				{
					model_mat,
					normal_mat,
					view_mat,
					projection_mat
				};

				bool should_draw_triangle = shader.VertexShader(v0, v1, v2, vs_shader_mats);

				if (!should_draw_triangle)
					continue;

				// if all three position components are inside the view frustum it doesn't need to be clipped
				if (v0.IsInsideViewFrustum() && v1.IsInsideViewFrustum() && v2.IsInsideViewFrustum())
				{
					TransformVertexScreenspace(v0);
					TransformVertexScreenspace(v1);
					TransformVertexScreenspace(v2);

					Triangle triangle
					{
						// store view projected W for perspective correct interpolation
						1.0f / v0.GetW(),
						1.0f / v1.GetW(),
						1.0f / v2.GetW(),
						v0.GetPosition(),
						v1.GetPosition(),
						v2.GetPosition(),
					};

					RasterizeTriangle(depthbuffer, triangle, color, shader);

					continue;
				}

				// store the verts in a buffer to clip them
				std::array<Vertex, 10> vertices_buffer = { v0, v1, v2 };
				uint8_t vertices_buffer_count = 3;

				bool clip_z_drawable = clipper.ClipVerticesAgainstAxis(vertices_buffer, &vertices_buffer_count, ClipAxis::AXIS_Z);

				if (!clip_z_drawable)
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
						// store view projected W for perspective correct interpolation
						1.0f / clipped_v0.GetW(),
						1.0f / clipped_v1.GetW(),
						1.0f / clipped_v2.GetW(),
						// screen space vertices
						clipped_v0.GetPosition(),
						clipped_v1.GetPosition(),
						clipped_v2.GetPosition()
					};

					RasterizeTriangle(depthbuffer, triangle, color, shader);
				}

			}

		}

		void Rasterizer::RasterizeTriangle(DepthBuffer& depthbuffer, const Triangle& triangle, const HSVColor& color, const IShader& shader)
		{
			int32_t bbox_min_x = std::min({ (int32_t)triangle.v0_screen.x, (int32_t)triangle.v1_screen.x, (int32_t)triangle.v2_screen.x });
			int32_t bbox_min_y = std::min({ (int32_t)triangle.v0_screen.y, (int32_t)triangle.v1_screen.y, (int32_t)triangle.v2_screen.y });
			int32_t bbox_max_x = std::max({ (int32_t)triangle.v0_screen.x, (int32_t)triangle.v1_screen.x, (int32_t)triangle.v2_screen.x });
			int32_t bbox_max_y = std::max({ (int32_t)triangle.v0_screen.y, (int32_t)triangle.v1_screen.y, (int32_t)triangle.v2_screen.y });
			
			// guaranteed to be outside the camera
			if (bbox_min_x >= renderer->GetFrameBufferWidth() || bbox_max_x < 0 || bbox_min_y >= renderer->GetFrameBufferHeight() || bbox_max_y < 0)
				return;

			Point2 bbox_min = Point2(
				std::max(0, bbox_min_x),
				std::max(0, bbox_min_y));

			Point2 bbox_max = Point2(
				std::min(bbox_max_x, (int32_t)renderer->GetFrameBufferWidth() - 1),
				std::min(bbox_max_y, (int32_t)renderer->GetFrameBufferHeight() - 1));

			Point2 point = Point2(bbox_min.x, bbox_min.y);

			TriangleEdge edge0 = TriangleEdge(triangle.v1_screen, triangle.v2_screen, point);
			TriangleEdge edge1 = TriangleEdge(triangle.v2_screen, triangle.v0_screen, point);
			TriangleEdge edge2 = TriangleEdge(triangle.v0_screen, triangle.v1_screen, point);
			TriangleEdge area = TriangleEdge(triangle.v0_screen, triangle.v1_screen, triangle.v2_screen);

			// degenerate triangle
			if (area.edgefunction_res == 0)
				return;

			for (uint16_t y = bbox_min.y; y <= bbox_max.y; y++)
			{
				int32_t edge0_mag_xy = edge0.edgefunction_res;
				int32_t edge1_mag_xy = edge1.edgefunction_res;
				int32_t edge2_mag_xy = edge2.edgefunction_res;

				for (uint16_t x = bbox_min.x; x <= bbox_max.x; x++)
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
					
						if (depthbuffer.GetValue(x, y) > z)
						{
							HSVColor out_color = color;
							shader.FragmentShader(out_color, triangle, barcoord0, barcoord1, barcoord2);

							depthbuffer.SetValue(x, y, z);
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

		void Rasterizer::DrawPixel(uint16_t x, uint16_t y, const HSVColor& color)
		{
			renderer->SetPixel(x, y, color);
		}

		void Rasterizer::SetModelMatrix(const Transform& model_transform)
		{
			model_mat = model_transform.GetTranslationMatrix() * (model_transform.GetRotationMatrix() * model_transform.GetScaleMatrix());
			// this should be the inverse transpose, but it's causing issues, TODO: investigate
			normal_mat = Matrix4(model_mat);
		}

		void Rasterizer::SetModelMatrix(const Matrix4& model_matrix)
		{
			model_mat = model_matrix;
			normal_mat = Matrix4(model_mat);
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