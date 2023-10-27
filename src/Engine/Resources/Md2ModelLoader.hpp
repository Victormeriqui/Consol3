#ifndef MD2MODELLOADER_HPP
#define MD2MODELLOADER_HPP

#include "Engine/Rendering/Animation.h"
#include "Engine/Rendering/Vertex.hpp"
#include "IModelLoader.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace Engine
{
    namespace Resources
    {
        using namespace Rendering;

        struct Md2Header
        {
            uint32_t magicnumber;
            uint32_t version;

            uint32_t skinwidth;
            uint32_t skinheight;

            uint32_t frame_size;
            uint32_t skins_count;
            uint32_t vertices_count;
            uint32_t textcoords_count;
            uint32_t triangles_count;
            uint32_t glcmds_count;
            uint32_t frames_count;
            uint32_t skins_offset;

            uint32_t textcoords_offset;
            uint32_t triangles_offset;
            uint32_t frames_offset;
            uint32_t glcmds_offset;
            uint32_t eof_offset;
        };

        class Md2ModelLoader : public IModelLoader
        {
        private:
            std::string GetNamePart(const std::string& frame_name) const;

        public:
            Md2ModelLoader()
            {
            }

            virtual bool LoadStaticModel(const std::string& filename, std::vector<Vertex>& out_vertices, std::vector<uint32_t>& out_indices, ModelLoadingOptions options) override;

            virtual bool LoadAnimatedModel(const std::string& filename, std::vector<Frame>& out_frames, std::vector<uint32_t>& out_indices, std::map<std::string, Animation>& out_animations, ModelLoadingOptions options) override;
        };
    }
}

#endif
