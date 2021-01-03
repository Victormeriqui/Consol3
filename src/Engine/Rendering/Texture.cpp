#include "Texture.hpp"

#include "../../Math/Util/MathUtil.hpp"

#include <array>
#include <fstream>
#include <sstream>

namespace Engine
{
	namespace Rendering
	{
		using namespace Display;
		using namespace Math;

		Texture::Texture() : imagebuffer(FrameBuffer<RGBColor>(0, 0))
		{
		}

		Texture::Texture(const FrameBuffer<RGBColor>& imagebuffer) : imagebuffer(imagebuffer)
		{
		}

		Texture::Texture(const std::string& filename, bool flip_y)
		{
			std::ifstream file_stream;

			file_stream.open(filename, std::ios::binary);

			if (!file_stream.is_open())
				return;

			const uint8_t bmp_header_bytes = 54;

			std::array<unsigned char, bmp_header_bytes> header;

			file_stream.read((char*)header.data(), header.size());

			uint32_t file_size = *reinterpret_cast<uint32_t*>(&header[2]);
			uint32_t offset	   = *reinterpret_cast<uint32_t*>(&header[10]);
			uint32_t width	   = *reinterpret_cast<uint32_t*>(&header[18]);
			uint32_t height	   = *reinterpret_cast<uint32_t*>(&header[22]);
			uint16_t depth	   = *reinterpret_cast<uint16_t*>(&header[28]);

			imagebuffer = FrameBuffer<RGBColor>(width, height);

			uint32_t data_size = width * height * 3;
			std::vector<unsigned char> data(data_size);
			file_stream.read((char*)data.data(), data.size());

			uint16_t x = 0;
			uint16_t y = flip_y ? height - 1 : 0;
			for (uint32_t i = 0; i < data.size(); i += 3)
			{
				unsigned char b = data[i];
				unsigned char g = data[i + 1];
				unsigned char r = data[i + 2];

				imagebuffer.SetValue(x++, y, RGBColor((uint8_t)r, (uint8_t)g, (uint8_t)b));
				if (x >= width)
				{
					y += flip_y ? -1 : 1;
					x = 0;
				}
			}

			file_stream.close();
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
