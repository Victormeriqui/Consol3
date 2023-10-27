#ifndef CLIPPER_HPP
#define CLIPPER_HPP

#include "Math/Vector3.hpp"
#include "Vertex.hpp"

#include <array>
#include <cstdint>

// TODO: refactor this class to use more readable datastructures
// important to avoid heap allocations since this should be parallel
namespace Engine
{
    namespace Rendering
    {
        using namespace Math;

        // planes to clip against, used to decide which component to clip from the vertices positions
        enum class ClipPlane
        {
            PLANE_POSITIVE_X,
            PLANE_POSITIVE_Y,
            PLANE_POSITIVE_Z,
            PLANE_NEGATIVE_X,
            PLANE_NEGATIVE_Y,
            PLANE_NEGATIVE_Z
        };

        // axis to clip against, used to decide which planes to clip the vertices with
        enum class ClipAxis
        {
            AXIS_X,
            AXIS_Y,
            AXIS_Z
        };

        class Clipper
        {
        private:
            // returns the correct component from the position for the given plane (ex: PLANE_POSITIVE_X or PLANE_NEGATIVE_X returns position.x)
            [[nodiscard]] inline float GetComponentForPlane(const Vector3& position, const ClipPlane plane) const;

            // solving the lerp equation to find the value that interpolates to the edge of the clipping plane
            [[nodiscard]] inline float GetLerpAmount(float point1, float point2, float start, float end);

            // clips the vertices currently in the buffer against the given plane, returns whether or not they should be drawn
            [[nodiscard]] bool ClipVerticesAgainstPlane(std::array<Vertex, 10>& out_vertices_buffer, uint8_t* out_vertices_buffer_count, const ClipPlane plane);

        public:
            Clipper();

            [[nodiscard]] bool ClipVerticesAgainstAxis(std::array<Vertex, 10>& out_vertices_buffer, uint8_t* out_vertices_buffer_count, const ClipAxis axis);
        };
    }
}

#endif
