#ifndef VERTEXBUFFER_HPP
#define VERTEXBUFFER_HPP

#include "Vertex.hpp"

#include <vector>

namespace Engine
{
	namespace Rendering
	{
		class VertexBuffer
		{
		private:

			std::vector<Vertex> vertices;
			std::vector<uint32_t> indices;

		public:
			VertexBuffer();
			VertexBuffer(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

			[[nodiscard]] const Vertex& GetVertex(int index) const;

			[[nodiscard]] const std::vector<Vertex>& GetVertices() const;
			[[nodiscard]] const std::vector<uint32_t>& GetIndices() const;
		};
	}
}

#endif