#include "ModelGenerator.hpp"

#include "Engine/Rendering/Vertex.hpp"
#include "Math/Vector3.hpp"
#include "NoiseGenerator.hpp"

#include <cmath>
#include <vector>

namespace Game
{
	using namespace Engine::Rendering;
	using namespace Math;

	ModelGenerator::ModelGenerator()
	{
	}

	StaticModel ModelGenerator::GeneratePlane(uint32_t tile_amount_x, uint32_t tile_amount_y, float noise_amount)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		Vector3 plane_normal(0, 1, 0);

		float tile_width  = 1.0f / tile_amount_x;
		float tile_height = 1.0f / tile_amount_y;

		float noise_x = 0.0f;
		float noise_y = 0.0f;

		// prevent vertex duplication
		for (uint32_t y = 0; y < (tile_amount_y + 1); y++)
		{
			for (uint32_t x = 0; x < (tile_amount_x + 1); x++)
			{
				float noise = GetPerlinNoise(Vector2(noise_x, noise_y));

				Vector2 texture_coordinates(Util::Lerp((float)x, 0, (float)(tile_amount_x + 1), 0.0f, 1.0f),
											Util::Lerp((float)y, 0, (float)(tile_amount_y + 1), 0.0f, 1.0f));

				vertices.push_back(
					Vertex(Vector3(x * tile_width, noise * (0.01f * noise_amount), y * tile_height), plane_normal, texture_coordinates));

				noise_x += 0.1f;
			}

			noise_y += 0.1f;
		}

		// only go until the second last row
		uint32_t col = 0;
		for (uint32_t i = 0; i < (vertices.size() - (tile_amount_x + 1)); i++)
		{
			// each iteration builds a full square, so the last column isn't needed
			if (col == tile_amount_x)
			{
				col = 0;
				continue;
			}
			col++;

			uint32_t next_y = i + tile_amount_x + 1;
			indices.push_back(i);
			indices.push_back(next_y);
			indices.push_back(i + 1);

			indices.push_back(next_y);
			indices.push_back(next_y + 1);
			indices.push_back(i + 1);
		}

		Vertex::CalculateNormals(vertices, indices);
		Vertex::CalculateTangents(vertices, indices);

		return StaticModel(vertices, indices);
	}

	std::optional<SphereEdge> ModelGenerator::GetExistingSphereEdge(const std::vector<SphereEdge>& edges,
																	uint32_t vert0_idx,
																	uint32_t vert1_idx) const
	{
		for (const SphereEdge& edge : edges)
		{
			if ((edge.vert0_idx == vert0_idx && edge.vert1_idx == vert1_idx) || (edge.vert0_idx == vert1_idx && edge.vert1_idx == vert0_idx))
				return std::optional<SphereEdge>(edge);
		}

		return std::nullopt;
	}

	uint32_t ModelGenerator::GetOrAddSubdividedVertex(std::vector<Vertex>& vertices,
													  std::vector<SphereEdge>& edges,
													  uint32_t vert0_idx,
													  uint32_t vert1_idx)
	{
		std::optional<SphereEdge> existing_edge = GetExistingSphereEdge(edges, vert0_idx, vert1_idx);

		if (existing_edge.has_value())
			return existing_edge.value().vert_mid_idx;

		SphereEdge new_edge = { vert0_idx,
								vert1_idx,
								// the new vertex will be pushed back, so it's index will be the last
								(uint32_t)vertices.size() };

		edges.push_back(new_edge);

		const Vertex& vert0 = vertices[vert0_idx];
		const Vertex& vert1 = vertices[vert1_idx];

		Vector3 new_mid				  = vert0.GetPosition().GetMiddleFrom(vert1.GetPosition()).Normalize();
		Vector2 new_mid_texturecoords = GetSphereTextureCoords(new_mid);

		vertices.push_back(Vertex(new_mid, new_mid, new_mid_texturecoords));

		// it's index is now the last one since it was just pushed back
		return (uint32_t)(vertices.size() - 1);
	}

	StaticModel ModelGenerator::GenerateSphere(uint8_t iterations)
	{
		//uint32_t vertex_count = CalculateVertexCount_Icosahedron(iterations);
		//uint32_t index_count  = CalculateIndexCount_Icosahedron(iterations);

		std::vector<SphereEdge> edges;
		std::vector<uint32_t> indices;
		std::vector<Vertex> vertices;

		for (const Vector3& pos : icosahedron_verts)
			vertices.push_back(Vertex(pos.GetNormalized(), pos.GetNormalized(), GetSphereTextureCoords(pos.GetNormalized())));

		for (uint32_t index : icosahedron_indices)
			indices.push_back(index);

		for (uint8_t i = 0; i < iterations; i++)
		{
			std::vector<uint32_t> new_indices;

			for (uint32_t idx = 0; idx < indices.size(); idx += 3)
			{
				uint32_t vert0_idx = indices[idx];
				uint32_t vert1_idx = indices[idx + 1];
				uint32_t vert2_idx = indices[idx + 2];

				uint32_t mid0 = GetOrAddSubdividedVertex(vertices, edges, vert0_idx, vert1_idx);
				uint32_t mid1 = GetOrAddSubdividedVertex(vertices, edges, vert1_idx, vert2_idx);
				uint32_t mid2 = GetOrAddSubdividedVertex(vertices, edges, vert2_idx, vert0_idx);

				new_indices.push_back(vert0_idx);
				new_indices.push_back(mid0);
				new_indices.push_back(mid2);

				new_indices.push_back(vert1_idx);
				new_indices.push_back(mid1);
				new_indices.push_back(mid0);

				new_indices.push_back(vert2_idx);
				new_indices.push_back(mid2);
				new_indices.push_back(mid1);

				new_indices.push_back(mid0);
				new_indices.push_back(mid1);
				new_indices.push_back(mid2);

				// remove original face, since it can now be represented by the new sub triangles
				indices.erase(indices.begin() + idx, indices.begin() + idx + 3);

				idx -= 3;
			}

			// add the new sub triangles
			for (uint32_t new_index : new_indices)
				indices.push_back(new_index);
		}

		/*// fixes texture overlap
		for (uint32_t idx = 0; idx < indices.size(); idx += 3)
		{
			Vertex& vert0 = vertices[indices[idx]];
			Vertex& vert1 = vertices[indices[idx + 1]];
			Vertex& vert2 = vertices[indices[idx + 2]];

			const Vector2& vert0_texturecoords = vert0.GetTextureCoords();
			const Vector2& vert1_texturecoords = vert1.GetTextureCoords();
			const Vector2& vert2_texturecoords = vert2.GetTextureCoords();

			if (vert1_texturecoords.x < 0.75 && vert0_texturecoords.x > 0.75)
				vert1.SetTextureCoords(Vector2(vert1_texturecoords.x + 1.0f, vert1_texturecoords.y));
			else if (vert1_texturecoords.x > 0.75 && vert0_texturecoords.x < 0.75)
				vert1.SetTextureCoords(Vector2(vert1_texturecoords.x - 1.0f, vert1_texturecoords.y));

			if (vert2_texturecoords.x < 0.75 && vert0_texturecoords.x > 0.75)
				vert2.SetTextureCoords(Vector2(vert2_texturecoords.x + 1.0f, vert2_texturecoords.y));
			else if (vert2_texturecoords.x > 0.75 && vert0_texturecoords.x < 0.75)
				vert2.SetTextureCoords(Vector2(vert2_texturecoords.x - 1.0f, vert2_texturecoords.y));
		}*/

		return StaticModel(vertices, indices);
	}

	uint32_t ModelGenerator::CalculateVertexCount_Icosahedron(int iterations) const
	{
		if (iterations == 0)
			return 12;

		return 4 * CalculateVertexCount_Icosahedron(iterations - 1);
	}

	uint32_t ModelGenerator::CalculateIndexCount_Icosahedron(int iterations) const
	{
		if (iterations == 0)
			return 60;

		return 4 * CalculateIndexCount_Icosahedron(iterations - 1);
	}

	Vector2 ModelGenerator::GetSphereTextureCoords(const Vector3& normal) const
	{
		// float u = std::atan2(normal.x, normal.z) / (2.0f * 3.1415926f) + 0.5f;
		// float v = normal.y * 0.5f + 0.5f;

		float u = (std::atan2(normal.z, normal.x) / (2.0f * 3.1415926f)) + 0.5f;
		float v = (std::asin(normal.y) / 3.1415926f) + 0.5f;

		return Vector2(u, v);
	}
}
