#include "AnimatedModel.hpp"
#include "../../Math/Util/MathUtil.hpp"

#include <cmath>

namespace Engine
{
	namespace Rendering
	{
		using namespace Math;

		AnimatedModel::AnimatedModel()
		{
		}

		AnimatedModel::AnimatedModel(const std::vector<Frame>& frames,
									 const std::vector<uint32_t>& indices,
									 const std::map<std::string, Animation>& animations) :
			frames(frames),
			indices(std::make_shared<std::vector<uint32_t>>()),
			animations(animations)
		{
			*this->indices.get() = indices;
		}

		VertexBuffer AnimatedModel::GetVertexBuffer(const std::string& animation_name, float interpolation)
		{
			if (animations.find(animation_name) == animations.end())
			{
				if (!frames.empty())
					return VertexBuffer(frames[0].vertices, indices);

				return VertexBuffer();
			}

			Animation anim = animations.at(animation_name);

			float calculated_frame = Util::Lerp(interpolation, (float)anim.start_frame, (float)anim.end_frame);

			float calculated_frame_intpart	 = 0;
			float calculated_frame_floatpart = std::modf(calculated_frame, &calculated_frame_intpart);

			// we are currently at a discrete frame, we can just return it
			if (calculated_frame_floatpart == 0)
				return VertexBuffer(frames[(uint32_t)calculated_frame_intpart].vertices, indices);

			// we must interpolate it

			uint32_t from_frame = (uint32_t)calculated_frame_intpart;
			uint32_t to_frame	= (uint32_t)calculated_frame_intpart + 1;

			std::vector<Vertex> vertices;

			for (uint32_t i = 0; i < frames[from_frame].vertices->size(); i++)
			{
				const Vertex& from_vert = frames[from_frame].vertices->at(i);
				const Vertex& to_vert	= frames[to_frame].vertices->at(i);

				vertices.push_back(std::move(from_vert.GetLerped(to_vert, calculated_frame_floatpart)));
			}

			// self stored vertices by the vertexbuffer
			return VertexBuffer(vertices, indices);
		}
	}
}
