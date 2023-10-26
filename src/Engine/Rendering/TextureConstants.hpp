#ifndef TEXTURECONSTANTS_HPP
#define TEXTURECONSTANTS_HPP

#include "Display/FrameBuffer.hpp"
#include "Display/RGBColor.hpp"
#include "Display/RGBColorConstants.hpp"
#include "Texture.hpp"

#include <memory>

namespace Engine
{
    namespace Rendering
    {
        using namespace Display;

        class TextureConstants
        {
        public:
            static std::shared_ptr<Texture> White()
            {
                static std::shared_ptr<Texture> ret = std::make_shared<Texture>(FrameBuffer<RGBColor>(1, 1, RGBConstants::White()), TextureWrapOptions::REPEAT);
                return ret;
            }

            static std::shared_ptr<Texture> FlatNormalMap()
            {
                static std::shared_ptr<Texture> ret = std::make_shared<Texture>(FrameBuffer<RGBColor>(1, 1, RGBColor(128, 128, 255)), TextureWrapOptions::REPEAT);
                return ret;
            }
        };

    }
}

#endif
