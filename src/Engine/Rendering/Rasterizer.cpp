#include "Rasterizer.hpp"

#include "../../Math/Point2.hpp"

#include <algorithm>
#include <immintrin.h>

namespace Engine
{
	namespace Rendering
	{
		using namespace Display;
		using namespace Math;
		using namespace Shaders;

		Rasterizer::Rasterizer(std::shared_ptr<IRenderer> renderer) :
			renderer(std::move(renderer)),
			clipper(Clipper()),
			viewport_mat(Matrix4().SetViewportMatrix(renderer->GetFrameBufferWidth(), renderer->GetFrameBufferHeight()))
		{
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

		void Rasterizer::DrawVertexBuffer(DepthBuffer& depthbuffer, const VertexBuffer& vertex_buffer, const RGBColor& color, IShader& shader)
		{
			ClipAndRasterize(depthbuffer, vertex_buffer, color, shader);
		}

		void Rasterizer::ClipAndRasterize(DepthBuffer& depthbuffer, const VertexBuffer& vertex_buffer, const RGBColor& color, IShader& shader)
		{
			for (uint32_t i = 0; i < vertex_buffer.GetIndices().size(); i += 3)
			{
				Vertex v0 = vertex_buffer.GetVertex(i);
				Vertex v1 = vertex_buffer.GetVertex(i + 1);
				Vertex v2 = vertex_buffer.GetVertex(i + 2);

				MVPTransform vs_shader_mats = { model_mat, normal_mat, view_mat, projection_mat };

				bool should_draw_triangle = shader.VertexShader(v0, v1, v2, vs_shader_mats);

				if (!should_draw_triangle)
					continue;

				// if all three position components are inside the view frustum it doesn't need to be clipped
				if (v0.IsInsideViewFrustum() && v1.IsInsideViewFrustum() && v2.IsInsideViewFrustum())
				{
					TransformVertexScreenspace(v0);
					TransformVertexScreenspace(v1);
					TransformVertexScreenspace(v2);

					Triangle triangle {
						// store view projected W for perspective correct interpolation
						1.0f / v0.GetW(), 1.0f / v1.GetW(), 1.0f / v2.GetW(), v0.GetPosition(), v1.GetPosition(), v2.GetPosition(),
					};

					RasterizeTriangle(depthbuffer, triangle, color, shader);

					continue;
				}

				// store the verts in a buffer to clip them
				std::array<Vertex, 10> vertices_buffer = { v0, v1, v2 };
				uint8_t vertices_buffer_count		   = 3;

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

					Triangle triangle { // store view projected W for perspective correct interpolation
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

		void Rasterizer::RasterizeTriangle(DepthBuffer& depthbuffer, const Triangle& triangle, const RGBColor& color, IShader& shader)
		{
			int32_t bbox_min_x = std::min({ (int32_t)triangle.v0_screen.x, (int32_t)triangle.v1_screen.x, (int32_t)triangle.v2_screen.x });
			int32_t bbox_min_y = std::min({ (int32_t)triangle.v0_screen.y, (int32_t)triangle.v1_screen.y, (int32_t)triangle.v2_screen.y });
			int32_t bbox_max_x = std::max({ (int32_t)triangle.v0_screen.x, (int32_t)triangle.v1_screen.x, (int32_t)triangle.v2_screen.x });
			int32_t bbox_max_y = std::max({ (int32_t)triangle.v0_screen.y, (int32_t)triangle.v1_screen.y, (int32_t)triangle.v2_screen.y });

			// guaranteed to be outside the camera
			if (bbox_min_x >= renderer->GetFrameBufferWidth() || bbox_max_x < 0 || bbox_min_y >= renderer->GetFrameBufferHeight() || bbox_max_y < 0)
				return;

			Point2 bbox_min = Point2(std::max(0, bbox_min_x), std::max(0, bbox_min_y));

			Point2 bbox_max = Point2(std::min(bbox_max_x, (int32_t)renderer->GetFrameBufferWidth() - 1),
									 std::min(bbox_max_y, (int32_t)renderer->GetFrameBufferHeight() - 1));

			Point2 point = Point2(bbox_min.x, bbox_min.y);

			SIMDTriangleEdge edge0 = SIMDTriangleEdge(triangle.v1_screen, triangle.v2_screen, point);
			SIMDTriangleEdge edge1 = SIMDTriangleEdge(triangle.v2_screen, triangle.v0_screen, point);
			SIMDTriangleEdge edge2 = SIMDTriangleEdge(triangle.v0_screen, triangle.v1_screen, point);

			TriangleEdge area = TriangleEdge(triangle.v0_screen, triangle.v1_screen, triangle.v2_screen);

			// degenerate triangle
			if (area.edgefunction_result == 0)
				return;
			for (uint16_t y = bbox_min.y; y <= bbox_max.y; y++)
			{
				__m128i edge0_mags_xy = edge0.edgefunction_results;
				__m128i edge1_mags_xy = edge1.edgefunction_results;
				__m128i edge2_mags_xy = edge2.edgefunction_results;

				for (uint16_t x = bbox_min.x; x < bbox_max.x; x += 4)
				{
					// OR the magnitudes for each edge on all parallel pixels
					__m128i joint_magnitudes = _mm_or_si128(_mm_or_si128(edge0_mags_xy, edge1_mags_xy), edge2_mags_xy);
					// 0 or !0 if the join magnitude is greater or equal 0, for each pixel
					__m128i are_inside_triangle = _mm_cmplt_epi32(_mm_set1_epi32(0), joint_magnitudes);

					// if any pixel is inside the triangle
					uint32_t should_draw = _mm_movemask_epi8(are_inside_triangle);

					if (should_draw)
					{
						__m128 barcoords0 = _mm_div_ps(_mm_cvtepi32_ps(edge0_mags_xy), _mm_set1_ps((float)area.edgefunction_result));
						__m128 barcoords1 = _mm_div_ps(_mm_cvtepi32_ps(edge1_mags_xy), _mm_set1_ps((float)area.edgefunction_result));
						// the sum of the 3 barycentric coords is 1, this avoids a division
						__m128 barcoords2 = _mm_sub_ps(_mm_set1_ps(1.0f), _mm_add_ps(barcoords0, barcoords1));

						// z = barcoord0 * triangle.v0_screen.z + barcoord1 * triangle.v1_screen.z + barcoord2 * triangle.v2_screen.z;
						__m128 z_values = _mm_add_ps(_mm_add_ps(_mm_mul_ps(barcoords0, _mm_set1_ps(triangle.v0_screen.z)),
																_mm_mul_ps(barcoords1, _mm_set1_ps(triangle.v1_screen.z))),
													 _mm_mul_ps(barcoords2, _mm_set1_ps(triangle.v2_screen.z)));

						__m128 depth_values = depthbuffer.GetValueRow(x, y).xyzw;

						// 0 or !0 if the depth test passed, for each pixel
						__m128i are_not_occluded = _mm_castps_si128(_mm_cmplt_ps(z_values, depth_values));

						__m128i out_mask = _mm_and_si128(are_not_occluded, are_inside_triangle);

						renderer->SetPixel(x, y, HSVColor(color));

						renderer->SetPixel(x + 1, y, HSVColor(color));

						renderer->SetPixel(x + 2, y, HSVColor(color));

						renderer->SetPixel(x + 3, y, HSVColor(color));
					}

					edge0_mags_xy = _mm_add_epi32(edge0_mags_xy, edge0.steps_delta_x);
					edge1_mags_xy = _mm_add_epi32(edge1_mags_xy, edge1.steps_delta_x);
					edge2_mags_xy = _mm_add_epi32(edge2_mags_xy, edge2.steps_delta_x);
				}

				edge0.edgefunction_results = _mm_add_epi32(edge0.edgefunction_results, edge0.steps_delta_y);
				edge1.edgefunction_results = _mm_add_epi32(edge1.edgefunction_results, edge1.steps_delta_y);
				edge2.edgefunction_results = _mm_add_epi32(edge2.edgefunction_results, edge2.steps_delta_y);
			}
		}

		void Rasterizer::DrawPixel(uint16_t x, uint16_t y, const RGBColor& color)
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
			model_mat  = model_matrix;
			normal_mat = Matrix4(model_mat);
		}

		void Rasterizer::SetViewMatrix(const Matrix4& view_matrix)
		{
			view_mat		 = view_matrix;
			inverse_view_mat = view_mat.GetInverted();
		}

		void Rasterizer::SetProjectionMatrix(const Matrix4& projection_matrix)
		{
			projection_mat		   = projection_matrix;
			inverse_projection_mat = projection_mat.GetInverted();
		}

		void Rasterizer::SetViewportMatrix(const Matrix4& viewport_matrix)

		{
			viewport_mat = viewport_matrix;
		}
	}
}
