#ifndef MODEL_HPP
#define MODEL_HPP

#include "Vertex.hpp"
#include "VertexBuffer.hpp"

#include <cstdint>
#include <vector>

namespace Engine
{
	namespace Rendering
	{
		class Model
		{
		private:
			VertexBuffer vertex_buffer;

		public:
			Model();
			Model(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

			[[nodiscard]] const VertexBuffer& GetVertexBuffer() const;
		};
	}
}

#endif
