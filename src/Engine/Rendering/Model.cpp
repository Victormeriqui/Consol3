#include "Model.hpp"

#include "Transform.hpp"
#include "Rasterizer.hpp"
#include "DepthBuffer.hpp"
#include "../../Math/Vector2.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

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

			std::vector<Vector3> normals;
			std::vector<Vector2> uvs;

			while (std::getline(file_stream, line))
			{
				line_split = SplitString(line, ' ');

	
				if (line_split.size() > 5 || line_split.empty())
					continue;

				if (line_split[0] == "v")
				{
					float x = std::stof(line_split[1]);
					float y = std::stof(line_split[2]);
					float z = std::stof(line_split[3]);

					vertices.push_back(Vertex(Vector3(x, y, z)));
				}

				if (line_split[0] == "vt")
				{
					float u = std::stof(line_split[1]);
					float v = std::stof(line_split[2]);
					
					uvs.push_back(Math::Vector2(u, v));
				}

				if (line_split[0] == "vn")
				{
					float x = std::stof(line_split[1]);
					float y = std::stof(line_split[2]);
					float z = std::stof(line_split[3]);

					normals.push_back(Vector3(x, y, z));
				}

				if (line_split[0] == "f")
				{
					// each face value can have textures or normal indices, we're only interested in the vertices (first before /)
					// - 1 for 0 based array indices
					std::vector<std::string> face0 = SplitString(line_split[1], '/');
					std::vector<std::string> face1 = SplitString(line_split[2], '/');
					std::vector<std::string> face2 = SplitString(line_split[3], '/');

					int slashes = std::count(line_split[1].begin(), line_split[1].end(), '/');
					bool has_empty_mid = line_split[1].find("//") != std::string::npos;

					uint32_t v0 = std::stoul(face0[0]) - 1;
					uint32_t v1 = std::stoul(face1[0]) - 1;
					uint32_t v2 = std::stoul(face2[0]) - 1;

					bool has_uv = false;
					uint32_t uv0 = 0;
					uint32_t uv1 = 0;
					uint32_t uv2 = 0;

					bool has_normal = false;
					uint32_t norm0 = 0;
					uint32_t norm1 = 0;
					uint32_t norm2 = 0;

					
					switch (slashes)
					{
					case 1:
						has_uv = true;
						uv0 = std::stoul(face0[1]) - 1;
						uv1 = std::stoul(face1[1]) - 1;
						uv2 = std::stoul(face2[1]) - 1;
						break;
					case 2:
						if (!has_empty_mid)
						{
							has_uv = true;
							uv0 = std::stoul(face0[1]) - 1;
							uv1 = std::stoul(face1[1]) - 1;
							uv2 = std::stoul(face2[1]) - 1;

							has_normal = true;
							norm0 = std::stoul(face0[2]) - 1;
							norm1 = std::stoul(face1[2]) - 1;
							norm2 = std::stoul(face2[2]) - 1;
						}
						else
						{
							has_normal = true;
							norm0 = std::stoul(face0[1]) - 1;
							norm1 = std::stoul(face1[1]) - 1;
							norm2 = std::stoul(face2[1]) - 1;
						}
						break;
					}
					
					indices.push_back(v0);
					indices.push_back(v1);
					indices.push_back(v2);

					if (has_uv)
					{
						vertices[v0].SetUV(uvs[uv0]);
						vertices[v1].SetUV(uvs[uv1]);
						vertices[v2].SetUV(uvs[uv2]);
					}
					if (has_normal)
					{
						vertices[v0].SetNormal(normals[norm0]);
						vertices[v1].SetNormal(normals[norm1]);
						vertices[v2].SetNormal(normals[norm2]);
					}
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

				//rasterizer.DrawShadedTriangle(depthbuffer, v0, v1, v2, HSVColor(randMToN(0, 360), 1.0f, 0.0f));
				//rasterizer.DrawTriangle(depthbuffer, v0, v1, v2, HSVColor(randMToN(0, 360), 1.0f, 1.0f));
				

				rasterizer.DrawTriangle(depthbuffer, v0, v1, v2, HSVColor(randMToN(0, 360), 1.0f, 1.0f));
			}
		}

		void Model::DrawModel(const Transform& transform, DepthBuffer& depthbuffer, Rasterizer& rasterizer, std::shared_ptr<Texture> texture, const HSVColor& color) const
		{
			srand(1004);
			rasterizer.SetModelMatrix(transform);
			rasterizer.SetActiveTexture(texture);

			for (uint32_t i = 0; i < indices.size(); i += 3)
			{
				Vertex v0 = vertices[indices[i]];
				Vertex v1 = vertices[indices[i + 1]];
				Vertex v2 = vertices[indices[i + 2]];

				rasterizer.DrawTexturedTriangle(depthbuffer, v0, v1, v2, HSVColor(randMToN(0, 360), 1.0f, 1.0f));
			}
		}
	}
}