#include "ResourceManager.hpp"
#include "../Rendering/Animation.h"
#include "../Rendering/StaticModel.hpp"

#include <map>

namespace Engine
{
	namespace Resources
	{
		ResourceManager::ResourceManager()
		{
			texture_cache.emplace("null", std::move(std::make_shared<Texture>()));
			static_model_cache.emplace("null", std::move(std::make_shared<StaticModel>()));
			animated_model_cache.emplace("null", std::move(std::make_shared<AnimatedModel>()));
		}

		std::string ResourceManager::GetFileExtension(const std::string& filename) const
		{
			std::string::size_type idx;

			idx = filename.rfind('.');

			if (idx == std::string::npos)
				return std::string();

			return filename.substr(idx + 1);
		}

		bool ResourceManager::LoadTexture(const std::string& filename, TextureLoadingOptions options)
		{
			if (texture_cache.find(filename) != texture_cache.end())
				return true;

			FrameBuffer<RGBColor> imagebuffer;
			bool success = false;

			std::string extension = GetFileExtension(filename);

			if (extension.empty())
				return false;

			if (extension == "bmp")
			{
				success = texture_loader_bmp.LoadTexture(filename, imagebuffer, options);

				if (!success)
					return false;

				texture_cache.emplace(filename, std::move(std::make_shared<Texture>(imagebuffer)));

				return true;
			}

			return false;
		}

		bool ResourceManager::LoadModel(const std::string& filename, NormalGenerationOptions options)
		{
			if (static_model_cache.find(filename) != static_model_cache.end() && animated_model_cache.find(filename) != animated_model_cache.end())
				return true;

			bool success = false;

			std::string extension = GetFileExtension(filename);

			if (extension.empty())
				return false;

			if (extension == "obj")
			{
				std::vector<Vertex> obj_vertices;
				std::vector<uint32_t> obj_indices;

				success = model_loader_obj.LoadStaticModel(filename, obj_vertices, obj_indices, options);

				if (!success)
					return false;

				static_model_cache.emplace(filename, std::move(std::make_shared<StaticModel>(obj_vertices, obj_indices)));

				return true;
			}
			else if (extension == "md2")
			{
				std::vector<Frame> md2_frames;
				std::vector<uint32_t> md2_indices;
				std::map<std::string, Animation> md2_animations;

				success = model_loader_md2.LoadAnimatedModel(filename, md2_frames, md2_indices, md2_animations, options);

				if (!success)
					return false;

				animated_model_cache.emplace(filename, std::move(std::make_shared<AnimatedModel>(md2_frames, md2_indices, md2_animations)));

				return true;
			}

			return false;
		}

		void ResourceManager::LoadTexture(const std::string& resource_name, const Texture& texture)
		{
			texture_cache.emplace(resource_name, std::move(std::make_shared<Texture>(texture)));
		}

		void ResourceManager::LoadModel(const std::string& resource_name, const StaticModel& model)
		{
			static_model_cache.emplace(resource_name, std::move(std::make_shared<StaticModel>(model)));
		}

		void ResourceManager::LoadModel(const std::string& resource_name, const AnimatedModel& model)
		{
			animated_model_cache.emplace(resource_name, std::move(std::make_shared<AnimatedModel>(model)));
		}

		std::shared_ptr<Texture> ResourceManager::GetLoadedTexture(const std::string& resource_name)
		{
			if (texture_cache.find(resource_name) == texture_cache.end())
				return texture_cache.at("null");

			return texture_cache.at(resource_name);
		}

		std::shared_ptr<StaticModel> ResourceManager::GetLoadedStaticModel(const std::string& resource_name)
		{
			if (static_model_cache.find(resource_name) == static_model_cache.end())
				return static_model_cache.at("null");

			return static_model_cache.at(resource_name);
		}

		std::shared_ptr<AnimatedModel> ResourceManager::GetLoadedAnimatedModel(const std::string& resource_name)
		{
			if (animated_model_cache.find(resource_name) == animated_model_cache.end())
				return animated_model_cache.at("null");

			return animated_model_cache.at(resource_name);
		}

	}
}
