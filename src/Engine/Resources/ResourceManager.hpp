#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include "../../Display/Framebuffer.hpp"
#include "../../Display/RGBColor.hpp"
#include "../Rendering/AnimatedModel.hpp"
#include "../Rendering/StaticModel.hpp"
#include "../Rendering/Texture.hpp"
#include "../Rendering/Vertex.hpp"
#include "BmpTextureLoader.hpp"
#include "IModelLoader.hpp"
#include "ITextureLoader.hpp"
#include "Md2ModelLoader.hpp"
#include "ObjModelLoader.hpp"

#include <cstdint>
#include <map>
#include <memory>
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

			bool LoadTexture(const std::string& filename,
							 TextureLoadingOptions load_options,
							 TextureWrapOptions wrap_options = TextureWrapOptions::BORDER);
			bool LoadModel(const std::string& filename, NormalGenerationOptions options);
			void LoadTexture(const std::string& resource_name, const Texture& texture);
			void LoadModel(const std::string& resource_name, const StaticModel& model);
			void LoadModel(const std::string& resource_name, const AnimatedModel& model);

			std::shared_ptr<Texture> GetLoadedTexture(const std::string& resource_name);
			std::shared_ptr<StaticModel> GetLoadedStaticModel(const std::string& resource_name);
			std::shared_ptr<AnimatedModel> GetLoadedAnimatedModel(const std::string& resource_name);
		};
	}
}

#endif
