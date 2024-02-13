#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "Math/Vector2I.hpp"
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
            TriangleEdge(const Vector2I& v0, const Vector2I& v1, const Vector2I& start_point)
            {
                // edge function: F(p) = (v0.y - v1.y)*start_point.x + (v1.x - v0.x)*start_point.y + (v0.x * v1.y - v0.y * v0.x)
                // this will have one sign if the point is to the left of the edge (v0 -> v1), and another sign if it is to the right
                // the actual sign will depend if the edges are defined clockwise or counter-clockwise

                // calculate each component without the point
                int32_t comp1 = v0.y - v1.y;
                int32_t comp2 = v1.x - v0.x;
                int32_t comp3 = v0.x * v1.y - v0.y * v1.x;

                // step deltas
                step_delta_x = comp1;
                step_delta_y = comp2;

                // calculate the whole edge function for the point
                edgefunction_res = (comp1 * start_point.x) + (comp2 * start_point.y) + comp3;
            }
        };

        // wrapper for the paramenters passed to the rasterizer and shader
        struct Triangle
        {
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
