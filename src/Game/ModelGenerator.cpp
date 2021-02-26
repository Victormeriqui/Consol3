#include "ModelGenerator.hpp"

#include "../Engine/Rendering/Vertex.hpp"
#include "../Math/Vector3.hpp"
#include "NoiseGenerator.hpp"

#include <cmath>
#include <map>
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
					std::move(Vertex(Vector3(x * tile_width, noise * (0.01f * noise_amount), y * tile_height), plane_normal, texture_coordinates)));

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

		vertices.push_back(std::move(Vertex(new_mid, new_mid, new_mid_texturecoords)));

		// it's index is now the last one since it was just pushed back
		return (uint32_t)(vertices.size() - 1);
	}

	StaticModel ModelGenerator::GenerateSphere(uint8_t iterations)
	{
		std::vector<SphereEdge> edges;
		std::vector<SphereTriangle> triangles;
		std::vector<uint32_t> indices;
		std::vector<Vertex> vertices;

		for (const Vector3& pos : icosahedron_verts)
			vertices.push_back(std::move(Vertex(pos.GetNormalized(), pos.GetNormalized(), GetSphereTextureCoords(pos.GetNormalized()))));

		for (const SphereTriangle& triangle : icosahedron_triangles)
			triangles.push_back(triangle);

		for (uint8_t it = 0; it < iterations; it++)
		{
			std::vector<SphereTriangle> new_triangles;

			for (uint32_t i = 0; i < triangles.size(); i++)
			{
				const SphereTriangle& triangle = triangles[i];

				uint32_t mid0 = GetOrAddSubdividedVertex(vertices, edges, triangle.vert0_idx, triangle.vert1_idx);
				uint32_t mid1 = GetOrAddSubdividedVertex(vertices, edges, triangle.vert1_idx, triangle.vert2_idx);
				uint32_t mid2 = GetOrAddSubdividedVertex(vertices, edges, triangle.vert2_idx, triangle.vert0_idx);

				new_triangles.push_back({ triangle.vert0_idx, mid0, mid2 });
				new_triangles.push_back({ triangle.vert1_idx, mid1, mid0 });
				new_triangles.push_back({ triangle.vert2_idx, mid2, mid1 });
				new_triangles.push_back({ mid0, mid1, mid2 });

				// remove the old triangle since it's now represented by the new subtriangles
				triangles.erase(triangles.begin() + i, triangles.begin() + i + 1);
				i--;
			}

			// add the new sub triangles
			for (const SphereTriangle& triangle : new_triangles)
				triangles.push_back(triangle);
		}

		//	CalculateWrappedUVIndices(vertices, triangles);

		for (const SphereTriangle& triangle : triangles)
		{
			indices.push_back(triangle.vert0_idx);
			indices.push_back(triangle.vert1_idx);
			indices.push_back(triangle.vert2_idx);
		}

		return StaticModel(vertices, indices);
	}

	Vector2 ModelGenerator::GetSphereTextureCoords(const Vector3& normal) const
	{
		float u = std::atan2(normal.x, normal.z) / (2.0f * 3.1415926f) + 0.5f;

		float v = normal.y * 0.5f + 0.5f;

		//	float u = (std::atan2(normal.z, normal.x) / (2.0f * 3.1415926f)) + 0.5f;
		// float v = (std::asin(normal.y) / 3.1415926f) + 0.5f;

		return Vector2(u, v);
	}

	void ModelGenerator::CalculateWrappedUVIndices(std::vector<Vertex>& vertices, std::vector<SphereTriangle>& triangles)
	{
		// index of the wrapped triangles in the triangle vector (NOT vertex indices)
		std::vector<uint32_t> wrapped_triangles_indices;

		for (uint32_t i = 0; i < triangles.size(); i++)
		{
			const SphereTriangle& triangle = triangles[i];

			const Vertex& vert0 = vertices[triangle.vert0_idx];
			const Vertex& vert1 = vertices[triangle.vert1_idx];
			const Vertex& vert2 = vertices[triangle.vert2_idx];

			const Vector3 vert0_uv = Vector3(vert0.GetTextureCoords().x, vert0.GetTextureCoords().y, 0);
			const Vector3 vert1_uv = Vector3(vert1.GetTextureCoords().x, vert1.GetTextureCoords().y, 0);
			const Vector3 vert2_uv = Vector3(vert2.GetTextureCoords().x, vert2.GetTextureCoords().y, 0);

			const Vector3 edge_uv0 = vert1_uv - vert0_uv;
			const Vector3 edge_uv1 = vert2_uv - vert0_uv;

			if (edge_uv0.GetCrossProduct(edge_uv1).z > 0.0f)
				wrapped_triangles_indices.push_back(i);
		}

		std::map<uint32_t, uint32_t> visited;

		for (int i = 0; i < wrapped_triangles_indices.size(); i++)
		{
			SphereTriangle& triangle = triangles[i];

			uint32_t vert0_idx = triangle.vert0_idx;
			uint32_t vert1_idx = triangle.vert1_idx;
			uint32_t vert2_idx = triangle.vert2_idx;

			Vertex& vert0 = vertices[vert0_idx];
			Vertex& vert1 = vertices[vert1_idx];
			Vertex& vert2 = vertices[vert2_idx];

			if (vert0.GetTextureCoords().x < 0.05f)
			{
				if (visited.find(vert0_idx) == visited.end())
				{
					// vert0.SetPosition(vert0.GetPosition() * 2);

					Vertex vert_new = Vertex(vert0.GetPosition(), vert0.GetNormal(), vert0.GetTextureCoords() + Vector2(5.0f, 0.0f));

					visited.insert_or_assign(vert0_idx, (uint32_t)vertices.size());
					triangle.vert0_idx = (uint32_t)vertices.size();

					vertices.push_back(vert_new);
				}
			}

			if (vert1.GetTextureCoords().x < 0.05f)
			{
				if (visited.find(vert1_idx) == visited.end())
				{
					// vert1.SetPosition(vert1.GetPosition() * 2);
					Vertex vert_new = Vertex(vert1.GetPosition(), vert1.GetNormal(), vert1.GetTextureCoords() + Vector2(5.0f, 0.0f));

					visited.insert_or_assign(vert1_idx, (uint32_t)vertices.size());
					triangle.vert1_idx = (uint32_t)vertices.size();

					vertices.push_back(vert_new);
				}
			}

			if (vert2.GetTextureCoords().x < 0.05f)
			{
				if (visited.find(vert2_idx) == visited.end())
				{
					// vert2.SetPosition(vert2.GetPosition() * 2);
					Vertex vert_new = Vertex(vert2.GetPosition(), vert2.GetNormal(), vert2.GetTextureCoords() + Vector2(5.0f, 0.0f));

					visited.insert_or_assign(vert2_idx, (uint32_t)vertices.size());
					triangle.vert2_idx = (uint32_t)vertices.size();

					vertices.push_back(vert_new);
				}
			}
		}
	}
}
