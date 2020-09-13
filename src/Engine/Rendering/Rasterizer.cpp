#include "Rasterizer.hpp"

#include "../../Display/FrameBuffer.hpp"
#include "../Math/Point2.hpp"
#include "../Math/Matrix4.hpp"
#include "Vertex.hpp"
#include "../../Display/Color.hpp"
#include "Transform.hpp"
#include "../../Display/FrameBuffer.hpp"

#include <algorithm>
#include <cstdint>

// TODO: implement this without consequences for the framebuffer
// this is done because the actual pixel size ratio in the framebuffer renderers might not be 1
// for example the 4x6 Raster Fonts with half block drawing becomes a 4x3 pixel size
// this fixes the squashing caused by the non 1 pixel size ratio, it's done here as it's easier before the framebuffer is already filled
// the consequence is that if we output the framebuffer to an actual 1x1 pixel size it will be squashed
#define FONT_WIDTH 4
#define FONT_HEIGHT 6
#define VIEWPORT_ASPECTRATIO FONT_WIDTH/(FONT_HEIGHT/2)

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

			mat[0][0] = width_h;  mat[0][1] = 0;                                 mat[0][2] = 0;     mat[0][3] = width_h - 0.5f;
			mat[1][0] = 0;        mat[1][1] = -height_h * VIEWPORT_ASPECTRATIO;  mat[1][2] = 0;     mat[1][3] = height_h - 0.5f;
			mat[2][0] = 0;        mat[2][1] = 0;                                 mat[2][2] = 1;     mat[2][3] = 0;
			mat[3][0] = 0;        mat[3][1] = 0;                                 mat[3][2] = 0;     mat[3][3] = 1;

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

		TriangleEdge::TriangleEdge(const Point2& v_a, const Point2& v_b, const Point2& start_point)
		{
			// edge function: F(p) = (v_a.y - v_b.y)*start_point.x + (v_b.x - v_a.x)*start_point.y + (v_a.x * v_b.y - v_a.y * v_a.x)
			// this will have one sign if the point is to the left of the edge (v_a -> v_b), and another sign if it is to the right
			// the actual sign will depend if the edges are defined clockwise or counter-clockwise

			// calculate each component without the point
			int comp1 = v_a.y - v_b.y;
			int comp2 = v_b.x - v_a.x;
			int comp3 = v_a.x * v_b.y - v_a.y * v_b.x;

			// step deltas
			step_delta_x = comp1;
			step_delta_y = comp2;

			// calculate the whole edge function for the point
			edgefunction_res = (comp1 * start_point.x) + (comp2 * start_point.y) + comp3;
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
			//	v0 = v1;
			//	v1 = temp;
				//return;
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

			TriangleEdge edge0 = TriangleEdge(v1_cliped, v2_cliped, point);
			TriangleEdge edge1 = TriangleEdge(v2_cliped, v0_cliped, point);
			TriangleEdge edge2 = TriangleEdge(v0_cliped, v1_cliped, point);

			for (uint16_t y = bb_min_y; y <= bb_max_y; y++)
			{
				int32_t edge0_mag_x = edge0.edgefunction_res;
				int32_t edge1_mag_x = edge1.edgefunction_res;
				int32_t edge2_mag_x = edge2.edgefunction_res;

				for (uint16_t x = bb_min_x; x <= bb_max_x; x++)
				{
					point.x = x;
					point.y = y;

					if ((edge0_mag_x | edge1_mag_x | edge2_mag_x) >= 0)
						framebuffer.SetPixel(x, y, color);

					edge0_mag_x += edge0.step_delta_x;
					edge1_mag_x += edge1.step_delta_x;
					edge2_mag_x += edge2.step_delta_x;
				}

				edge0.edgefunction_res += edge0.step_delta_y;
				edge1.edgefunction_res += edge1.step_delta_y;
				edge2.edgefunction_res += edge2.step_delta_y;
			}
		}

		void Rasterizer::SetModelMatrix(const Transform& model_transform)
		{
			model_mat = model_transform.GetTranslationMatrix() * (model_transform.GetRotationMatrix() * model_transform.GetScaleMatrix());
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