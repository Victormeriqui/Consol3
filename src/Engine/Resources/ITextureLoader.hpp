#ifndef ITEXTURELOADER_HPP
#define ITEXTURELOADER_HPP

#include "Display/FrameBuffer.hpp"
#include "Display/RGBColor.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace Engine
{
    namespace Resources
    {
        using namespace Display;

        enum class TextureLoadingOptions
        {
            DEFAULT,
            FLIP_X,
            FLIP_Y,
            FLIP_XY
        };

        class ITextureLoader
        {
        protected:
            ITextureLoader()
            {
            }

        public:
            [[nodiscard]] virtual bool LoadTexture(const std::string& filename, FrameBuffer<RGBColor>& out_imagebuffer, TextureLoadingOptions options) = 0;
        };
    }
}

#endif
