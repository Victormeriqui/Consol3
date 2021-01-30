#ifndef STATICMODEL_HPP
#define STATICMODEL_HPP

#include "Vertex.hpp"
#include "VertexBuffer.hpp"

#include <cstdint>
#include <vector>

namespace Engine
{
	namespace Rendering
	{
		class StaticModel
		{
		private:
			std::shared_ptr<std::vector<Vertex>> vertices;
			std::shared_ptr<std::vector<uint32_t>> indices;

			VertexBuffer vertex_buffer;

		public:
			StaticModel();
			StaticModel(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

			const VertexBuffer& GetVertexBuffer() const;
		};
	}
}

#endif
