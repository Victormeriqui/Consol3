#include "VertexBuffer.hpp"

namespace Engine
{
    namespace Rendering
    {
        VertexBuffer::VertexBuffer()
        {
        }

        VertexBuffer::VertexBuffer(std::shared_ptr<std::vector<Vertex>> vertices, std::shared_ptr<std::vector<uint32_t>> indices) : vertices(std::move(vertices)), indices(std::move(indices))
        {
        }

        VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices, std::shared_ptr<std::vector<uint32_t>> indices) : indices(std::move(indices)), selfstored_vertices(vertices)
        {
        }

        const Vertex& VertexBuffer::GetVertex(int index) const
        {
            if (selfstored_vertices.empty())
                return vertices->at(indices->at(index));

            return selfstored_vertices.at(indices->at(index));
        }

        const std::vector<Vertex>& VertexBuffer::GetVertices() const
        {
            if (selfstored_vertices.empty())
                return *vertices.get();

            return selfstored_vertices;
        }

        const std::vector<uint32_t>& VertexBuffer::GetIndices() const
        {
            return *indices.get();
        }
    }
}
