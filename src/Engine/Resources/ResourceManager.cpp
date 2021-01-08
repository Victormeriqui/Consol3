#include "ResourceManager.hpp"

namespace Engine
{
	namespace Resources
	{
		ResourceManager::ResourceManager()
		{
			texture_cache.emplace("null", std::move(std::make_shared<Texture>()));
			model_cache.emplace("null", std::move(std::make_shared<Model>()));
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
			if (model_cache.find(filename) != model_cache.end())
				return true;

			std::vector<Vertex> vertices;
			std::vector<uint32_t> indices;
			bool success = false;

			std::string extension = GetFileExtension(filename);

			if (extension.empty())
				return false;

			if (extension == "obj")
			{
				success = model_loader_obj.LoadModel(filename, vertices, indices, options);

				if (!success)
					return false;

				model_cache.emplace(filename, std::move(std::make_shared<Model>(vertices, indices)));

				return true;
			}

			return false;
		}

		void ResourceManager::LoadTexture(const std::string& resource_name, const Texture& texture)
		{
			texture_cache.emplace(resource_name, std::move(std::make_shared<Texture>(texture)));
		}

		void ResourceManager::LoadModel(const std::string& resource_name, const Model& model)
		{
			model_cache.emplace(resource_name, std::move(std::make_shared<Model>(model)));
		}

		std::shared_ptr<Texture> ResourceManager::GetLoadedTexture(const std::string& resource_name)
		{
			if (texture_cache.find(resource_name) == texture_cache.end())
				return texture_cache.at("null");

			return texture_cache.at(resource_name);
		}

		std::shared_ptr<Model> ResourceManager::GetLoadedModel(const std::string& resource_name)
		{
			if (model_cache.find(resource_name) == model_cache.end())
				return model_cache.at("null");

			return model_cache.at(resource_name);
		}

	}
}
