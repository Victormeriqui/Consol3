#include "VertexBuffer.hpp"

namespace Engine
{
	namespace Rendering
	{
		VertexBuffer::VertexBuffer()
		{
		}

		VertexBuffer::VertexBuffer(std::shared_ptr<std::vector<Vertex>> vertices, std::shared_ptr<std::vector<uint32_t>> indices) :
			vertices(std::move(vertices)),
			indices(std::move(indices))
		{
		}

		const Vertex& VertexBuffer::GetVertex(int index) const
		{
			return vertices->at(indices->at(index));
		}

		const std::vector<Vertex>& VertexBuffer::GetVertices() const
		{
			return *vertices.get();
		}

		const std::vector<uint32_t>& VertexBuffer::GetIndices() const
		{
			return *indices.get();
		}
	}
}
