#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Display/FrameBuffer.hpp"
#include "Display/RGBColor.hpp"
#include "Math/Vector2.hpp"

#include <cstdint>
#include <string>

namespace Engine
{
    namespace Rendering
    {
        using namespace Display;
        using namespace Math;

        enum class TextureWrapOptions
        {
            REPEAT,
            CLAMP,
            BORDER
        };

        class Texture
        {
        private:
            FrameBuffer<RGBColor> imagebuffer;

            TextureWrapOptions wrap_options;

        public:
            Texture();
            Texture(const FrameBuffer<RGBColor>& imagebuffer, TextureWrapOptions wrap_options);

            [[nodiscard]] FrameBuffer<RGBColor> GetBuffer() const;
            [[nodiscard]] RGBColor GetColorFromTextureCoords(const Vector2& texture_coords);
            [[nodiscard]] RGBColor GetColorFromTextureCoords(float x, float y);
        };
    }
}

#endif
