#ifndef MODEL_HPP
#define MODEL_HPP

#include "Vertex.hpp"
#include "VertexBuffer.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace Engine
{
	namespace Rendering
	{
		class Model
		{
		private:
			VertexBuffer vertex_buffer;

			inline std::vector<std::string> SplitString(std::string string, char delimiter) const;

			void CalculateNormals(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

		public:
			Model();
			Model(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
			Model(const std::string& filename);

			[[nodiscard]] const VertexBuffer& GetVertexBuffer() const;
		};
	}
}

#endif