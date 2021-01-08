#include "Model.hpp"

namespace Engine
{
	namespace Rendering
	{
		Model::Model()
		{
		}

		Model::Model(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) : vertex_buffer(vertices, indices)
		{
		}

		const VertexBuffer& Model::GetVertexBuffer() const
		{
			return vertex_buffer;
		}
	}
}
