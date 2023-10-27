#ifndef ANIMATEDMODEL_HPP
#define ANIMATEDMODEL_HPP

#include "Animation.h"
#include "VertexBuffer.hpp"

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace Engine
{
    namespace Rendering
    {
        using namespace Math;

        class AnimatedModel
        {
        private:
            std::vector<Frame> frames;
            std::shared_ptr<std::vector<uint32_t>> indices;

            std::map<std::string, Animation> animations;

        public:
            AnimatedModel();
            AnimatedModel(const std::vector<Frame>& frames, const std::vector<uint32_t>& indices, const std::map<std::string, Animation>& animations);

            [[nodiscard]] const Animation& GetAnimation(const std::string& name) const;

            VertexBuffer GetVertexBuffer(const std::string& animation_name, float interpolation);
        };
    }
}

#endif
