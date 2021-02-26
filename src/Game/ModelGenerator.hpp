#ifndef MODELGENERATOR_HPP
#define MODELGENERATOR_HPP

#include "../Engine/Rendering/StaticModel.hpp"
#include "../Math/Vector2.hpp"
#include "../Math/Vector3.hpp"

#include <cstdint>
#include <map>
#include <optional>

namespace Game
{
	using namespace Engine;
	using namespace Rendering;

	struct SphereEdge
	{
		uint32_t vert0_idx;
		uint32_t vert1_idx;
		uint32_t vert_mid_idx;
	};

	struct SphereTriangle
	{
		uint32_t vert0_idx;
		uint32_t vert1_idx;
		uint32_t vert2_idx;
	};

	static Vector3 icosahedron_verts[12] = { Vector3(-1, 0, 1), Vector3(1, 0, 1),  Vector3(-1, 0, -1), Vector3(1, 0, -1),
											 Vector3(0, 1, 1),	Vector3(0, 1, -1), Vector3(0, -1, 1),  Vector3(0, -1, -1),
											 Vector3(1, 1, 0),	Vector3(-1, 1, 0), Vector3(1, -1, 0),  Vector3(-1, -1, 0) };

	static uint32_t icosahedron_indices[60] = {
		1, 4,  0, 4,  9, 0, 4, 5,  9, 8, 5, 4,	1, 8, 4, 1,	 10, 8, 10, 3, 8, 8, 3,	 5, 3, 2, 5, 3,	 7, 2,
		3, 10, 7, 10, 6, 7, 6, 11, 7, 6, 0, 11, 6, 1, 0, 10, 1,	 6, 11, 0, 9, 2, 11, 9, 5, 2, 9, 11, 2, 7,
	};

	static SphereTriangle icosahedron_triangles[20] = {
		{ 1, 4, 0 },  { 4, 9, 0 },	{ 4, 5, 9 },  { 8, 5, 4 },	{ 1, 8, 4 }, { 1, 10, 8 }, { 10, 3, 8 }, { 8, 3, 5 },  { 3, 2, 5 }, { 3, 7, 2 },
		{ 3, 10, 7 }, { 10, 6, 7 }, { 6, 11, 7 }, { 6, 0, 11 }, { 6, 1, 0 }, { 10, 1, 6 }, { 11, 0, 9 }, { 2, 11, 9 }, { 5, 2, 9 }, { 11, 2, 7 },
	};

	class ModelGenerator
	{
	private:
		[[nodiscard]] inline std::optional<SphereEdge> GetExistingSphereEdge(const std::vector<SphereEdge>& edges,
																			 uint32_t vert0_idx,
																			 uint32_t vert1_idx) const;

		[[nodiscard]] inline uint32_t GetOrAddSubdividedVertex(std::vector<Vertex>& vertices,
															   std::vector<SphereEdge>& edges,
															   uint32_t vert0_idx,
															   uint32_t vert1_idx);

		[[nodiscard]] Vector2 GetSphereTextureCoords(const Vector3& normal) const;

		[[nodiscard]] void CalculateWrappedUVIndices(std::vector<Vertex>& vertices, std::vector<SphereTriangle>& triangles);

	public:
		ModelGenerator();

		StaticModel GeneratePlane(uint32_t tile_amount_x, uint32_t tile_amount_y, float noise_amount);
		StaticModel GenerateSphere(uint8_t iterations);
	};
}

#endif
