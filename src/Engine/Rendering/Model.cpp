#include "Model.hpp"

#include "Transform.hpp"
#include "Rasterizer.hpp"

#include <string>
#include <fstream>
#include <sstream>

namespace Engine
{
	namespace Rendering
	{

		Model::Model()
		{
		}

		uint32_t random(uint32_t min, uint32_t max)
		{
			return rand() % (max + 1 - min) + min;
		}

		Model::Model(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
		{
			this->vertices = vertices;
			this->indices = indices;

		}

		inline std::vector<std::string> Model::SplitString(std::string string, char delimiter) const
		{
			std::vector<std::string> str_split;
			std::istringstream str_stream = std::istringstream(string);

			std::string str_part;
			while (std::getline(str_stream, str_part, delimiter))
			{
				// happens if the string is split by multiple instances of the delimiter
				if (str_part.empty())
					continue;

				str_split.push_back(str_part);
			}

			return str_split;
		}

		Model::Model(std::string filename)
		{
			std::ifstream file_stream;

			file_stream.open(filename);

			std::string line;
			
			if (!file_stream.is_open())
				return;

			std::vector<std::string> line_split;

			while (std::getline(file_stream, line))
			{
				line_split = SplitString(line, ' ');
				
				// ignore lines that arent "v 1 2 3" or "f 1 2 3"
				if (line_split.size() != 4)
					continue;

				if (line_split[0] == "v")
				{
					float x = std::stof(line_split[1]);
					float y = std::stof(line_split[2]);
					float z = std::stof(line_split[3]);

					vertices.push_back(Vertex(Vector3(x, y, z)));
				}

				if (line_split[0] == "f")
				{
					// each face value can have textures or normal indices, we're only interested in the vertices (first before /)
					// - 1 for 0 based array indices
					uint32_t v0 = std::stoul(SplitString(line_split[1], '/')[0]) - 1;
					uint32_t v1 = std::stoul(SplitString(line_split[2], '/')[0]) - 1;
					uint32_t v2 = std::stoul(SplitString(line_split[3], '/')[0]) - 1;

					indices.push_back(v0);
					indices.push_back(v1);
					indices.push_back(v2);
				}

			}

			file_stream.close();
		}

		void Model::CalculateNormals() const
		{
			//TODO: do this
		}

		std::vector<Vertex> Model::GetVertices() const
		{
			return vertices;
		}

		std::vector<uint32_t> Model::GetIndices() const
		{
			return indices;
		}

		void Model::DrawModel(const Transform& transform, Rasterizer& rasterizer) const
		{
			rasterizer.SetModelMatrix(transform);

			uint32_t color = 0;
			srand(123154);
			for (uint32_t i = 0; i < indices.size(); i += 3)
			{
				color = random(0, 0xffffff);
				Vertex v0 = vertices[indices[i]];
				Vertex v1 = vertices[indices[i + 1]];
				Vertex v2 = vertices[indices[i + 2]];

				rasterizer.RasterizeTriangle(v0, v1, v2, color);
			}
		}

	}
}