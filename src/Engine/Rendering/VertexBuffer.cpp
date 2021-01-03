#include "VertexBuffer.hpp"

namespace Engine
{
	namespace Rendering
	{
		VertexBuffer::VertexBuffer()
		{
		}

		VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
		{
			this->vertices = vertices;
			this->indices = indices;
		}

		const Vertex& VertexBuffer::GetVertex(int index) const
		{
			return vertices[indices[index]];
		}

		const std::vector<Vertex>& VertexBuffer::GetVertices() const
		{
			return vertices;
		}

		const std::vector<uint32_t>& VertexBuffer::GetIndices() const
		{
			return indices;
		}
	}
}
