#include "Texture.hpp"

namespace Engine
{
	namespace Rendering
	{
		Texture::Texture() : imagebuffer(FrameBuffer<RGBColor>(0, 0))
		{
		}

		Texture::Texture(const FrameBuffer<RGBColor>& imagebuffer) : imagebuffer(imagebuffer)
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
			uint16_t text_x = (uint16_t)(x * imagebuffer.GetWidth());
			uint16_t text_y = (uint16_t)(y * imagebuffer.GetHeight());

			// TODO: remove this
			if (text_x >= imagebuffer.GetWidth() || text_y >= imagebuffer.GetHeight())
				return RGBColor(0, 0, 0);

			return imagebuffer.GetValue(text_x, text_y);
		}
	}
}
