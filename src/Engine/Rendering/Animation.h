#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "Math/Vector3.hpp"
#include "Vertex.hpp"
#include "VertexBuffer.hpp"

#include <memory>
#include <string>

namespace Engine
{
	namespace Rendering
	{
		struct Frame
		{
			std::string name;

			std::shared_ptr<std::vector<Vertex>> vertices;
		};

		struct Animation
		{
			uint32_t start_frame;
			uint32_t end_frame;
		};
	}
}

#endif
