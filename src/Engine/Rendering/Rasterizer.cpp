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

		Vertex& Rasterizer::TransformVertexMVP(Vertex& vertex)
		{
			vertex *= model_mat;
			vertex.TransformNormals(normal_mat);

			vertex *= view_mat;
			vertex *= projection_mat;

			return vertex;
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

		void Rasterizer::DrawFilledTriangle(DepthBuffer& depthbuffer, Vertex v0, Vertex v1, Vertex v2, HSVColor color)
		{
			TransformVertexMVP(v0);
			TransformVertexMVP(v1);
			TransformVertexMVP(v2);

			if (IsBackface(v0.GetPosition(), v1.GetPosition(), v2.GetPosition()))
				return;

			// if all three position components are inside the view frustum it doesn't need to be clipped
			if (v0.IsInsideViewFrustum() && v1.IsInsideViewFrustum() && v2.IsInsideViewFrustum())
			{
				RasterizeFilledTriangle(depthbuffer, v0, v1, v2, color);
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
				RasterizeFilledTriangle(depthbuffer, vertices_buffer[0], vertices_buffer[i], vertices_buffer[i + 1], color);
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

		void Rasterizer::RasterizeFilledTriangle(DepthBuffer& depthbuffer, Vertex v0, Vertex v1, Vertex v2, HSVColor color)
		{
			TransformVertexScreenspace(v0);
			TransformVertexScreenspace(v1);
			TransformVertexScreenspace(v2);

			Point2 v0_cliped = Point2(v0.GetPosition());
			Point2 v1_cliped = Point2(v1.GetPosition());
			Point2 v2_cliped = Point2(v2.GetPosition());

			uint16_t bb_min_x = std::min({ v0_cliped.x, v1_cliped.x, v2_cliped.x });
			uint16_t bb_min_y = std::min({ v0_cliped.y, v1_cliped.y, v2_cliped.y });
			uint16_t bb_max_x = std::max({ v0_cliped.x, v1_cliped.x, v2_cliped.x });
			uint16_t bb_max_y = std::max({ v0_cliped.y, v1_cliped.y, v2_cliped.y });

			Vector2 point = Vector2(bb_min_x, bb_min_y);

			TriangleEdge edge0 = TriangleEdge(v1_cliped, v2_cliped, point);
			TriangleEdge edge1 = TriangleEdge(v2_cliped, v0_cliped, point);
			TriangleEdge edge2 = TriangleEdge(v0_cliped, v1_cliped, point);
			TriangleEdge area = TriangleEdge(v0_cliped, v1_cliped, v2_cliped);

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
						float barcord0 = edge0_mag_xy / (float)area.edgefunction_res;
						float barcord1 = edge1_mag_xy / (float)area.edgefunction_res;
						float barcord2 = edge2_mag_xy / (float)area.edgefunction_res;

						float z = barcord0 * v0.GetPosition().z + barcord1 * v1.GetPosition().z + barcord2 * v2.GetPosition().z;

						if (depthbuffer.GetDepth(x, y) > z)
						{
							renderer->SetPixel(x, y, color);
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

		//TODO: get rid of code duplication in this class
		void Rasterizer::DrawLitTriangle(DepthBuffer& depthbuffer, Vertex v0, Vertex v1, Vertex v2, HSVColor color)
		{
			TransformVertexMVP(v0);
			TransformVertexMVP(v1);
			TransformVertexMVP(v2);

			if (IsBackface(v0.GetPosition(), v1.GetPosition(), v2.GetPosition()))
				return;

			// if all three position components are inside the view frustum it doesn't need to be clipped
			if (v0.IsInsideViewFrustum() && v1.IsInsideViewFrustum() && v2.IsInsideViewFrustum())
			{
				RasterizeLitTriangle(depthbuffer, v0, v1, v2, color);
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
				RasterizeLitTriangle(depthbuffer, vertices_buffer[0], vertices_buffer[i], vertices_buffer[i + 1], color);
		}

		void Rasterizer::RasterizeLitTriangle(DepthBuffer& depthbuffer, Vertex v0, Vertex v1, Vertex v2, HSVColor color)
		{
			TransformVertexScreenspace(v0);
			TransformVertexScreenspace(v1);
			TransformVertexScreenspace(v2);

			Point2 v0_cliped = Point2(v0.GetPosition());
			Point2 v1_cliped = Point2(v1.GetPosition());
			Point2 v2_cliped = Point2(v2.GetPosition());

			uint16_t bb_min_x = std::min({ v0_cliped.x, v1_cliped.x, v2_cliped.x });
			uint16_t bb_min_y = std::min({ v0_cliped.y, v1_cliped.y, v2_cliped.y });
			uint16_t bb_max_x = std::max({ v0_cliped.x, v1_cliped.x, v2_cliped.x });
			uint16_t bb_max_y = std::max({ v0_cliped.y, v1_cliped.y, v2_cliped.y });

			Vector2 point = Vector2(bb_min_x, bb_min_y);

			TriangleEdge edge0 = TriangleEdge(v1_cliped, v2_cliped, point);
			TriangleEdge edge1 = TriangleEdge(v2_cliped, v0_cliped, point);
			TriangleEdge edge2 = TriangleEdge(v0_cliped, v1_cliped, point);
			TriangleEdge area = TriangleEdge(v0_cliped, v1_cliped, v2_cliped);

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
						float barcord0 = edge0_mag_xy / (float)area.edgefunction_res;
						float barcord1 = edge1_mag_xy / (float)area.edgefunction_res;
						float barcord2 = edge2_mag_xy / (float)area.edgefunction_res;

						float z = barcord0 * v0.GetPosition().z + barcord1 * v1.GetPosition().z + barcord2 * v2.GetPosition().z;
						float light_amount = barcord0 * lighting_system->GetLightAmountAt(v0) +
							barcord1 * lighting_system->GetLightAmountAt(v1)
							+ barcord2 * lighting_system->GetLightAmountAt(v2);

						if (depthbuffer.GetDepth(x, y) > z)
						{
							HSVColor lit_color = HSVColor(color.hue, color.saturation, std::min(1.0f, color.value + light_amount + 0.02f));
							renderer->SetPixel(x, y, lit_color);
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
		}

		void Rasterizer::SetProjectionMatrix(const Matrix4& projection_matrix)
		{
			projection_mat = projection_matrix;
		}

		void Rasterizer::SetViewportMatrix(const Matrix4& viewport_matrix)
		{
			viewport_mat = viewport_matrix;
		}
	}
}