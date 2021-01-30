#ifndef ANIMATEDMODEL_HPP
#define ANIMATEDMODEL_HPP

#include "../../Math/Vector3.hpp"
#include "Vertex.hpp"
#include "VertexBuffer.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace Engine
{
	namespace Rendering
	{
		using namespace Math;

		struct Frame
		{
			std::string name;
			Vector3 scale;
			Vector3 translation;

			std::vector<Vertex> vertices;
		};

		struct Animation
		{
			uint32_t start_frame;
			uint32_t frame_count;
			uint16_t fps;
		};

		class AnimatedModel
		{
		private:
			std::vector<Frame> frames;
			std::vector<uint32_t> indices;

			std::map<std::string, Animation> animations;

		public:
			AnimatedModel();

			const VertexBuffer& GetVertexBuffer(const std::string& animation_name, float interpolation) const;
		};
	}
}

#endif
