#ifndef BMPTEXTURELOADER_HPP
#define BMPTEXTURELOADER_HPP

#include "Display/FrameBuffer.hpp"
#include "Display/RGBColor.hpp"
#include "ITextureLoader.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace Engine
{
    namespace Resources
    {
        using namespace Display;

        class BmpTextureLoader : public ITextureLoader
        {
        private:
        public:
            BmpTextureLoader()
            {
            }

            virtual bool LoadTexture(const std::string& filename, FrameBuffer<RGBColor>& out_imagebuffer, TextureLoadingOptions options) override;
        };
    }
}

#endif
