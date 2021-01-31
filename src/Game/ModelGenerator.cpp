#include "ModelGenerator.hpp"

#include "../Engine/Rendering/Vertex.hpp"
#include "../Math/Vector3.hpp"
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

	StaticModel ModelGenerator::GenerateSphere(uint8_t detail)
	{
		// TODO: do this
		return StaticModel();
	}
}
