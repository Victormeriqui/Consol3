#ifndef MODELGENERATOR_HPP
#define MODELGENERATOR_HPP

#include "Engine/Rendering/StaticModel.hpp"
#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"

#include <cstdint>
#include <map>
#include <optional>

namespace Game
{
    using namespace Engine;
    using namespace Rendering;

    static Vector3 icosahedron_verts[12] = { Vector3(-1, 0, 1), Vector3(1, 0, 1),   Vector3(-1, 0, -1), Vector3(1, 0, -1), Vector3(0, 1, 1),  Vector3(0, 1, -1),
                                             Vector3(0, -1, 1), Vector3(0, -1, -1), Vector3(1, 1, 0),   Vector3(-1, 1, 0), Vector3(1, -1, 0), Vector3(-1, -1, 0) };

    static uint32_t icosahedron_indices[60] = {
        1, 4, 0, 4, 9, 0, 4, 5, 9, 8, 5, 4, 1, 8, 4, 1, 10, 8, 10, 3, 8, 8, 3, 5, 3, 2, 5, 3, 7, 2, 3, 10, 7, 10, 6, 7, 6, 11, 7, 6, 0, 11, 6, 1, 0, 10, 1, 6, 11, 0, 9, 2, 11, 9, 5, 2, 9, 11, 2, 7,
    };

    struct SphereEdge
    {
        uint32_t vert0_idx;
        uint32_t vert1_idx;
        uint32_t vert_mid_idx;
    };

    class ModelGenerator
    {
    private:
        // 12, 42, 320, 642, 2562, 10242, 40962, 163842
        [[nodiscard]] inline uint32_t CalculateVertexCount_Icosahedron(int iterations) const;
        // https://www.wolframalpha.com/input/?i=60,+240,+3840,+15360,+61440,+245760,+983040
        // 60, 240, 960, 3840, 15360, 61440, 245760, 983040
        [[nodiscard]] inline uint32_t CalculateIndexCount_Icosahedron(int iterations) const;

        [[nodiscard]] inline std::optional<SphereEdge> GetExistingSphereEdge(const std::vector<SphereEdge>& edges, uint32_t vert0_idx, uint32_t vert1_idx) const;

        [[nodiscard]] inline uint32_t GetOrAddSubdividedVertex(std::vector<Vertex>& vertices, std::vector<SphereEdge>& edges, uint32_t vert0_idx, uint32_t vert1_idx);

        [[nodiscard]] Vector2 GetSphereTextureCoords(const Vector3& normal) const;

    public:
        ModelGenerator();

        StaticModel GeneratePlane(uint32_t tile_amount_x, uint32_t tile_amount_y, float noise_amount);
        StaticModel GenerateSphere(uint8_t iterations);
    };
}

#endif
