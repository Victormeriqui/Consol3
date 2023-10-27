#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include "BmpTextureLoader.hpp"
#include "Display/FrameBuffer.hpp"
#include "Display/RGBColor.hpp"
#include "Engine/Rendering/AnimatedModel.hpp"
#include "Engine/Rendering/StaticModel.hpp"
#include "Engine/Rendering/Texture.hpp"
#include "Engine/Rendering/Vertex.hpp"
#include "IModelLoader.hpp"
#include "ITextureLoader.hpp"
#include "Md2ModelLoader.hpp"
#include "ObjModelLoader.hpp"

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <string>

namespace Engine
{
    namespace Resources
    {
        using namespace Rendering;

        class ResourceManager
        {
        private:
            std::map<std::string, std::shared_ptr<StaticModel>> static_model_cache;
            std::map<std::string, std::shared_ptr<AnimatedModel>> animated_model_cache;
            std::map<std::string, std::shared_ptr<Texture>> texture_cache;

            ObjModelLoader model_loader_obj;
            Md2ModelLoader model_loader_md2;
            BmpTextureLoader texture_loader_bmp;

            [[nodiscard]] std::string GetFileExtension(const std::string& filename) const;

        public:
            ResourceManager();

            bool LoadTexture(const std::string& filename, TextureLoadingOptions load_options, TextureWrapOptions wrap_options = TextureWrapOptions::BORDER);
            bool LoadModel(const std::string& filename, ModelLoadingOptions options);
            void LoadTexture(const std::string& resource_name, const Texture& texture);
            void LoadModel(const std::string& resource_name, const StaticModel& model);
            void LoadModel(const std::string& resource_name, const AnimatedModel& model);

            [[nodiscard]] std::optional<std::shared_ptr<Texture>> GetLoadedTexture(const std::string& resource_name);
            [[nodiscard]] std::optional<std::shared_ptr<StaticModel>> GetLoadedStaticModel(const std::string& resource_name);
            [[nodiscard]] std::optional<std::shared_ptr<AnimatedModel>> GetLoadedAnimatedModel(const std::string& resource_name);
        };
    }
}

#endif
