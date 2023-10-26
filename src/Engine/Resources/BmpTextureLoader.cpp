#include "BmpTextureLoader.hpp"

#include <array>
#include <fstream>
#include <sstream>

namespace Engine
{
    namespace Resources
    {
        using namespace Display;

        bool BmpTextureLoader::LoadTexture(const std::string& filename, FrameBuffer<RGBColor>& out_imagebuffer, TextureLoadingOptions options)
        {
            std::ifstream file_stream;

            file_stream.open(filename, std::ios::binary);

            if (!file_stream.is_open())
                return false;

            const uint8_t bmp_header_bytes = 54;

            std::array<unsigned char, bmp_header_bytes> header;

            file_stream.read((char*)header.data(), header.size());

            // uint32_t file_size = *reinterpret_cast<uint32_t*>(&header[2]);
            // uint32_t offset	   = *reinterpret_cast<uint32_t*>(&header[10]);
            uint32_t width  = *reinterpret_cast<uint32_t*>(&header[18]);
            uint32_t height = *reinterpret_cast<uint32_t*>(&header[22]);
            // uint16_t depth	   = *reinterpret_cast<uint16_t*>(&header[28]);

            out_imagebuffer = FrameBuffer<RGBColor>(width, height);

            uint64_t data_size = width * height * 3;
            std::vector<unsigned char> data(data_size);
            data.resize(data_size);
            file_stream.read((char*)data.data(), sizeof(unsigned char) * data_size);

            bool flip_x = options == TextureLoadingOptions::FLIP_X || options == TextureLoadingOptions::FLIP_XY;
            bool flip_y = options == TextureLoadingOptions::FLIP_Y || options == TextureLoadingOptions::FLIP_XY;

            uint16_t x = flip_x ? width - 1 : 0;
            uint16_t y = flip_y ? height - 1 : 0;

            for (uint64_t i = 0; i < data_size; i += 3)
            {
                unsigned char b = data[i];
                unsigned char g = data[i + 1];
                unsigned char r = data[i + 2];

                out_imagebuffer.SetValue(x, y, RGBColor((uint8_t)r, (uint8_t)g, (uint8_t)b));

                if (flip_x)
                {
                    if (x == 0)
                    {
                        y += flip_y ? -1 : 1;
                        x = width;
                    }
                }
                else
                {
                    if (x == width - 1)
                    {
                        y += flip_y ? -1 : 1;
                        x = -1;
                    }
                }

                x += flip_x ? -1 : 1;
            }

            file_stream.close();

            return true;
        }

    }
}
