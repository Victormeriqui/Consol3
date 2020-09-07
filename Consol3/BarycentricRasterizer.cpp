#include "BarycentricRasterizer.hpp"

#include "FrameBuffer.hpp"
#include "Point2.hpp"

#include <algorithm>

namespace Engine
{
	namespace Rendering
	{
		BarycentricRasterizer::BarycentricRasterizer(FrameBuffer& framebuffer) : AbstractRasterizer(framebuffer)
		{
		}

		int32_t BarycentricRasterizer::GetEdgeMagnituteToPoint(const Point2& v_a, const Point2& v_b, const Point2& point)
		{
			return (point.x - v_a.x) * (v_b.y - v_a.y) - (point.y - v_a.y) * (v_b.x - v_a.x);
		}

		void BarycentricRasterizer::RasterizeTriangle(Vertex v0, Vertex v1, Vertex v2, Color color)
		{
			TransformVertexMVP(v1);
			TransformVertexMVP(v1);
			TransformVertexMVP(v2);

			const Vector3& v0_pos = v0.GetPosition();
			const Vector3& v1_pos = v1.GetPosition();
			const Vector3& v2_pos = v2.GetPosition();

			//clip triangle

			Point2 v0_cliped = Point2(v0_pos);
			Point2 v1_cliped = Point2(v1_pos);
			Point2 v2_cliped = Point2(v2_pos);
			
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

					if (edge1to2_mag >= 0 && edge2to0_mag >= 0 && edge0to1_mag >= 0)
						framebuffer.SetPixel(x, y, 0xffffff);
				}

			}

		}
	}
}