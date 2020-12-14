#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "../../Math/Point2.hpp"
#include "../../Math/Vector2.hpp"
#include "Vertex.hpp"

#include <cstdint>

namespace Engine
{
	namespace Rendering
	{
		// a triangle edge for rasterization, initializes with the edge function value for the given point
		struct TriangleEdge
		{
			// the x component step of the edge function for each pixel to the right
			int32_t step_delta_x;
			// the y component step of the edge function for each pixel down
			int32_t step_delta_y;

			// the value of the edge function will be initialized with the result for the starting
			// pixel, remaining values can be stepped with the deltas
			int32_t edgefunction_res;

			// calculates the edge function for a specific point (starting point), and fills in the step deltas taken from the function's components
			TriangleEdge(const Point2& v_a, const Point2& v_b, const Point2& point);
		};

		// wrapper for the paramenters passed to the rasterizer and shader
		struct Triangle
		{
			// original vertices for interpolation
			const Vertex& v0;
			const Vertex& v1;
			const Vertex& v2;

			// we multiply with 1/w because its faster to multiply than to divide multiple times
			float v0_oneoverw;
			float v1_oneoverw;
			float v2_oneoverw;

			const Vector3& v0_screen;
			const Vector3& v1_screen;
			const Vector3& v2_screen;
		};
	}
}


#endif