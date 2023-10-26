#include "Texture.hpp"

#include <algorithm>

namespace Engine
{
    namespace Rendering
    {
        Texture::Texture() : imagebuffer(FrameBuffer<RGBColor>(0, 0))
        {
        }

        Texture::Texture(const FrameBuffer<RGBColor>& imagebuffer, TextureWrapOptions wrap_options) : imagebuffer(imagebuffer), wrap_options(wrap_options)
        {
        }

        FrameBuffer<RGBColor> Texture::GetBuffer() const
        {
            return imagebuffer;
        }

        RGBColor Texture::GetColorFromTextureCoords(const Vector2& texture_coords)
        {
            return GetColorFromTextureCoords(texture_coords.x, texture_coords.y);
        }

        RGBColor Texture::GetColorFromTextureCoords(float x, float y)
        {
            switch (wrap_options)
            {
            case TextureWrapOptions::CLAMP:
                x = std::clamp(x, 0.0f, 1.0f);
                y = std::clamp(y, 0.0f, 1.0f);
                break;
            case TextureWrapOptions::REPEAT:
                if (x < 0)
                    x *= -1;
                if (y < 0)
                    y *= -1;

                x = x - (uint32_t)x;
                y = y - (uint32_t)y;
                break;
            case TextureWrapOptions::BORDER:
                if ((x < 0.0f || x > 1.0f) || (y < 0.0f || y > 1.0f))
                    return RGBColor(255, 1, 1);
            }

            uint16_t text_x = (uint16_t)(x * imagebuffer.GetWidth());
            uint16_t text_y = (uint16_t)(y * imagebuffer.GetHeight());

            // TODO: remove this
            if (text_x >= imagebuffer.GetWidth() || text_y >= imagebuffer.GetHeight())
                return RGBColor(0, 0, 0);

            //	return RGBColor(x * 255, 0, 0);
            return imagebuffer.GetValue(text_x, text_y);
        }
    }
}
