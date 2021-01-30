#ifndef FILEUTILS_HPP
#define FILEUTILS_HPP

#include "../../Rendering/Vertex.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>

namespace Engine
{
	namespace Resources
	{
		namespace Util
		{
			using namespace Rendering;

			inline void CalculateNormals(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
			{
				for (uint32_t i = 0; i < indices.size() - 3; i += 3)
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

			inline std::vector<std::string> SplitString(std::string string, char delimiter)
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

			inline uint32_t CountLinesStartsWith(std::ifstream& file_stream, const std::string& startswith)
			{
				if (!file_stream.is_open())
					return -1;

				uint32_t count = 0;

				std::string line;
				while (std::getline(file_stream, line))
				{
					if (line.rfind(startswith, 0) == 0)
						count++;
				}

				file_stream.clear();
				file_stream.seekg(0);

				return count;
			}

		}
	}
}

#endif
