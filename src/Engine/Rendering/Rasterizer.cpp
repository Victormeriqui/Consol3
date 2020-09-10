#include "Rasterizer.hpp"

#include "../../Display/FrameBuffer.hpp"
#include "../Math/Point2.hpp"
#include "../Math/Matrix4.hpp"
#include "Vertex.hpp"
#include "../../Display/Color.hpp"
#include "Transform.hpp"
#include "../../Display/FrameBuffer.hpp"

#include <algorithm>

using namespace Display;

namespace Engine
{
	namespace Rendering
	{
		Rasterizer::Rasterizer(FrameBuffer& framebuffer) : framebuffer(framebuffer)
		{
			float width_h = framebuffer.GetWidth() / 2.0f;
			float height_h = framebuffer.GetHeight() / 2.0f;
			float mat[4][4];

			mat[0][0] = width_h;  mat[0][1] = 0;          mat[0][2] = 0;     mat[0][3] = width_h - 0.5f;
			mat[1][0] = 0;        mat[1][1] = -height_h;  mat[1][2] = 0;     mat[1][3] = height_h - 0.5f;
			mat[2][0] = 0;        mat[2][1] = 0;          mat[2][2] = 1;     mat[2][3] = 0;
			mat[3][0] = 0;        mat[3][1] = 0;          mat[3][2] = 0;     mat[3][3] = 1;

			viewport_mat = Matrix4(mat);
		}

		Vertex& Rasterizer::TransformVertexMVP(Vertex& vertex)
		{
			vertex *= model_mat;
			vertex *= view_mat;
			vertex *= projection_mat;

			return vertex;
		}

		Vertex& Rasterizer::TransformVertexNDC(Vertex& vertex)
		{

			vertex *= viewport_mat;
			vertex.PerspectiveDivide();
			return vertex;
		}

		int32_t Rasterizer::GetEdgeMagnituteToPoint(const Point2& v_a, const Point2& v_b, const Point2& point)
		{
			return (point.x - v_a.x) * (v_b.y - v_a.y) - (point.y - v_a.y) * (v_b.x - v_a.x);
		}

		void Rasterizer::RasterizeTriangle(Vertex v0, Vertex v1, Vertex v2, Color color)
		{

			TransformVertexMVP(v0);
			TransformVertexMVP(v1);
			TransformVertexMVP(v2);

			TransformVertexNDC(v0);
			TransformVertexNDC(v1);
			TransformVertexNDC(v2);
			
			if (v0.GetPosition().x > v1.GetPosition().x)
			{
				Vertex temp = v0;
				v0 = v1;
				v1 = temp;
			}

			//clip triangle

			Point2 v0_cliped = Point2(v0.GetPosition());
			Point2 v1_cliped = Point2(v1.GetPosition());
			Point2 v2_cliped = Point2(v2.GetPosition());
			
			uint16_t bb_min_x = std::min({ v0_cliped.x, v1_cliped.x, v2_cliped.x });
			uint16_t bb_min_y = std::min({ v0_cliped.y, v1_cliped.y, v2_cliped.y });
			uint16_t bb_max_x = std::max({ v0_cliped.x, v1_cliped.x, v2_cliped.x });
			uint16_t bb_max_y = std::max({ v0_cliped.y, v1_cliped.y, v2_cliped.y });

			Vector2 point = Vector2(bb_min_x, bb_min_y);

			for (uint16_t y = bb_min_y; y <= bb_max_y; y++)
			{
				for (uint16_t x = bb_min_x; x <= bb_max_x; x++)
				{
					point.x = x;
					point.y = y;

					int32_t edge1to2_mag = GetEdgeMagnituteToPoint(v1_cliped, v2_cliped, point);
					int32_t edge2to0_mag = GetEdgeMagnituteToPoint(v2_cliped, v0_cliped, point);
					int32_t edge0to1_mag = GetEdgeMagnituteToPoint(v0_cliped, v1_cliped, point);

					if (edge1to2_mag <= 0 && edge2to0_mag <= 0 && edge0to1_mag <= 0)
						framebuffer.SetPixel(x, y, color);
			
				}

			}
		}

		void Rasterizer::SetModelMatrix(const Transform& model_transform)
		{
			model_mat = model_transform.GetTransformationMatrix();
		}

		void Rasterizer::SetModelMatrix(const Matrix4& model_matrix)
		{
			model_mat = model_matrix;
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