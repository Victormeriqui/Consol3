#include "StaticModel.hpp"

#include <algorithm>

namespace Engine
{
	namespace Rendering
	{
		StaticModel::StaticModel()
		{
		}

		StaticModel::StaticModel(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) :
			vertices(std::make_shared<std::vector<Vertex>>()),
			indices(std::make_shared<std::vector<uint32_t>>()),
			vertex_buffer(this->vertices, this->indices)
		{
			*this->vertices.get() = vertices;
			*this->indices.get()  = indices;
		}

		const VertexBuffer& StaticModel::GetVertexBuffer() const
		{
			return vertex_buffer;
		}
	}
}
