#include "Model.hpp"

#include "Transform.hpp"
#include "Rasterizer.hpp"
#include "DepthBuffer.hpp"

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

			CalculateNormals();
		}

		void Model::CalculateNormals()
		{
			for (unsigned int i = 0; i < indices.size() - 3; i += 3)
			{
				Vertex& v0 = vertices[indices[i]];
				Vertex& v1 = vertices[indices[i + 1]];
				Vertex& v2 = vertices[indices[i + 2]];

				Vector3 edge0 = v1.GetPosition() - v0.GetPosition();
				Vector3 edge1 = v2.GetPosition() - v0.GetPosition();

				Vector3 normal = edge0.GetCrossProduct(edge1);
				normal.Normalize();

				// each adjacent face to each vertex contributes to the vertex normal
				v0.SetNormal(v0.GetNormal() + normal);
				v1.SetNormal(v1.GetNormal() + normal);
				v2.SetNormal(v2.GetNormal() + normal);
			}

			for (Vertex& vert : vertices)
				vert.SetNormal(vert.GetNormal().GetNormalized());
		}

		std::vector<Vertex> Model::GetVertices() const
		{
			return vertices;
		}

		std::vector<uint32_t> Model::GetIndices() const
		{
			return indices;
		}
		float randMToN(float M, float N)
		{
			return M + (rand() / (RAND_MAX / (N - M)));
		}

		void Model::DrawModel(const Transform& transform, DepthBuffer& depthbuffer, Rasterizer& rasterizer, const HSVColor& color) const
		{
			srand(1004);
			rasterizer.SetModelMatrix(transform);

			for (uint32_t i = 0; i < indices.size(); i += 3)
			{
				Vertex v0 = vertices[indices[i]];
				Vertex v1 = vertices[indices[i + 1]];
				Vertex v2 = vertices[indices[i + 2]];

				rasterizer.DrawLitTriangle(depthbuffer, v0, v1, v2, HSVColor(randMToN(0, 360), 1.0f, 0.0f));
			}
		}
	}
}