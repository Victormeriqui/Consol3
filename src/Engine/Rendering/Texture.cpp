#include "Texture.hpp"

#include "../../Display/RGBColor.hpp"
#include "../../Display/Framebuffer.hpp"
#include "../../Math/Vector2.hpp"
#include "../../Math/Util/MathUtil.hpp"

#include <cstdint>
#include <string>
#include <fstream>
#include <sstream>
#include <array>

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

		Texture::Texture(const std::string& filename)
		{
			std::ifstream file_stream;

			file_stream.open(filename, std::ios::binary);

			if (!file_stream.is_open())
				return;
			
			const uint8_t bmp_header_bytes = 54;

			std::array<unsigned char, bmp_header_bytes> header;

			file_stream.read((char*)header.data(), header.size());

			uint32_t file_size = *reinterpret_cast<uint32_t*>(&header[2]);
			uint32_t offset = *reinterpret_cast<uint32_t*>(&header[10]);
			uint32_t width = *reinterpret_cast<uint32_t*>(&header[18]);
			uint32_t height = *reinterpret_cast<uint32_t*>(&header[22]);
			uint16_t depth = *reinterpret_cast<uint16_t*>(&header[28]);
			
			imagebuffer = FrameBuffer<RGBColor>(width, height);

			uint32_t data_size = width * height * 3;
			std::vector<unsigned char> data(data_size);
			file_stream.read((char*)data.data(), data.size());

			uint16_t x = 0;
			uint16_t y = 0;// height - 1;
			for (uint32_t i = 0; i < data.size(); i += 3)
			{

				unsigned char b = data[i];
				unsigned char g = data[i + 1];
				unsigned char r = data[i + 2];

				imagebuffer.SetPixel(x++, y, RGBColor((uint8_t)r, (uint8_t)g, (uint8_t)b));
				if (x >= width)
				{
					y++;
					x = 0;
				}
			}

			file_stream.close();
		}

		FrameBuffer<RGBColor> Texture::GetBuffer() const
		{
			return imagebuffer;
		}

		RGBColor Texture::GetColorFromUV(const Vector2& uv)
		{
			return GetColorFromUV(uv.x, uv.y);
		}

		RGBColor Texture::GetColorFromUV(float u, float v)
		{
			//uint16_t x = Util::LerpCast<uint16_t>(u, 0, imagebuffer.GetWidth());
			//uint16_t y = Util::LerpCast<uint16_t>(v, 0, imagebuffer.GetHeight());

			uint16_t x = u * imagebuffer.GetWidth();
			uint16_t y = v * imagebuffer.GetHeight();

			// TODO: remove this
			if (x >= imagebuffer.GetWidth() || y >= imagebuffer.GetHeight())
				return RGBColor(0, 0, 0);

			return imagebuffer.GetPixel(x, y);
		}
	}
}