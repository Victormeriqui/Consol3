#include "Clipper.hpp"

#include "Vertex.hpp"
#include "../../Math/Vector3.hpp"

#include <cstdint>
#include <array>

namespace Engine
{
	namespace Rendering
	{
		using namespace Math;

		Clipper::Clipper()
		{
		}

		float Clipper::GetComponentForPlane(const Vector3& position, const ClipPlane plane) const
		{
			switch (plane)
			{
			case ClipPlane::PLANE_POSITIVE_X:
			case ClipPlane::PLANE_NEGATIVE_X:
				return position.x;
			case ClipPlane::PLANE_POSITIVE_Y:
			case ClipPlane::PLANE_NEGATIVE_Y:
				return position.y;
			case ClipPlane::PLANE_POSITIVE_Z:
			case ClipPlane::PLANE_NEGATIVE_Z:
				return position.z;
			default:
				break;
			}

			return 0;
		}

		float Clipper::GetLerpAmount(float point1, float point2, float start, float end)
		{
			//lerp equation:  lerpedpoint = v1*(1-L) + v2*L, where L = lerp val
			//solve for lerpedpoint = 1, get lerp val
			//L = (lerpedpoint-v1) / (lerpedpoint-v1)-(lerpedpoint-v2)
			//L = (1-v1) / (1-v1)-(1-v2)
			//lerpedpoint for clipping will be w

			float numerator = point1 - start;
			float denominator = numerator - (point2 - end);

			return numerator / denominator;
		}

		bool Clipper::ClipVerticesAgainstPlane(std::array<Vertex, 10>& out_vertices_buffer, uint8_t* out_vertices_buffer_count, const ClipPlane plane)
		{
			if (out_vertices_buffer_count == 0)
				return false;

			// temporary buffer to store the results
			std::array<Vertex, 10> temporary_vertices_buffer;
			uint8_t temporary_vertices_buffer_count = 0;

			bool negative_clipping = false;
			// if we're clipping against the negative plane we're doing it against -w
			switch (plane)
			{
			case ClipPlane::PLANE_NEGATIVE_X:
			case ClipPlane::PLANE_NEGATIVE_Y:
			case ClipPlane::PLANE_NEGATIVE_Z:
				negative_clipping = true;
				break;
			default:
				break;
			}

			Vertex prev_vert = out_vertices_buffer[*out_vertices_buffer_count - 1];

			float prev_point_component = GetComponentForPlane(prev_vert.GetPosition(), plane);

			if (!negative_clipping)
				prev_point_component = -prev_point_component;

			// point is inside if its between -w and w, since this is done per plane, only one side is checked per call
			bool prev_point_is_inside = prev_point_component <= prev_vert.GetW();

			for (uint8_t i = 0; i < *out_vertices_buffer_count; i++)
			{
				Vertex cur_vert = out_vertices_buffer[i];

				float cur_point_component = GetComponentForPlane(cur_vert.GetPosition(), plane);
				
				if (!negative_clipping)
					cur_point_component = -cur_point_component;
				
				bool cur_point_is_inside = cur_point_component <= cur_vert.GetW();

				// one of the vertices is outside the plane, interpolate to the edge
				if ((prev_point_is_inside && !cur_point_is_inside) || (!prev_point_is_inside && cur_point_is_inside))
				{
					// interpolate the vert to the edge and add it to the results
					float lerp_amount = GetLerpAmount(prev_vert.GetW(), cur_vert.GetW(), prev_point_component, cur_point_component);
					temporary_vertices_buffer[temporary_vertices_buffer_count++] = prev_vert.GetLerped(cur_vert, lerp_amount);
				}

				if (cur_point_is_inside)
					temporary_vertices_buffer[temporary_vertices_buffer_count++] = cur_vert;

				prev_vert = cur_vert;
				prev_point_component = cur_point_component;
				prev_point_is_inside = cur_point_is_inside;
			}

			out_vertices_buffer = temporary_vertices_buffer;
			*out_vertices_buffer_count = temporary_vertices_buffer_count;

			return *out_vertices_buffer_count > 0;
		}

		bool Clipper::ClipVerticesAgainstAxis(std::array<Vertex, 10>& out_vertices_buffer, uint8_t* out_vertices_buffer_count, const ClipAxis axis)
		{
			if (*out_vertices_buffer_count == 0)
				return false;

			ClipPlane positive_plane = ClipPlane();
			ClipPlane negative_plane = ClipPlane();

			switch (axis)
			{
			case ClipAxis::AXIS_X:
				positive_plane = ClipPlane::PLANE_POSITIVE_X;
				negative_plane = ClipPlane::PLANE_NEGATIVE_X;
				break;
			case ClipAxis::AXIS_Y:
				positive_plane = ClipPlane::PLANE_POSITIVE_Y;
				negative_plane = ClipPlane::PLANE_NEGATIVE_Y;
				break;
			case ClipAxis::AXIS_Z:
				positive_plane = ClipPlane::PLANE_POSITIVE_Z;
				negative_plane = ClipPlane::PLANE_NEGATIVE_Z;
				break;
			default:
				break;
			}

			bool positive_clip_drawable = ClipVerticesAgainstPlane(out_vertices_buffer, out_vertices_buffer_count, positive_plane);

			if (!positive_clip_drawable)
				return false;

			bool negative_clip_drawable = ClipVerticesAgainstPlane(out_vertices_buffer, out_vertices_buffer_count, negative_plane);

			return negative_clip_drawable;
		}
	}
}
