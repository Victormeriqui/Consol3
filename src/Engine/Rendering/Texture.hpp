#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "../../Display/RGBColor.hpp"
#include "../../Display/Framebuffer.hpp"
#include "../../Math/Vector2.hpp"

#include <cstdint>
#include <string>

namespace Engine
{
	namespace Rendering
	{
		using namespace Display;
		using namespace Math;

		class Texture
		{
		private:
			FrameBuffer<RGBColor> imagebuffer;

		public:
			Texture();
			Texture(const FrameBuffer<RGBColor>& imagebuffer);
			Texture(const std::string& filename, bool flip_y = false);

			[[nodiscard]] FrameBuffer<RGBColor> GetBuffer() const;
			[[nodiscard]] RGBColor GetColorFromTextureCoords(const Vector2& texture_coords);
			[[nodiscard]] RGBColor GetColorFromTextureCoords(float x, float y);
		};
	}
}

#endif