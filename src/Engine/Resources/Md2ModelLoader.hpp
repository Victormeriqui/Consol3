#ifndef MD2MODELLOADER_HPP
#define MD2MODELLOADER_HPP

#include "../Rendering/Vertex.hpp"
#include "IModelLoader.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace Engine
{
	namespace Resources
	{
		using namespace Rendering;

		class Md2ModelLoader : public IModelLoader
		{
		public:
			Md2ModelLoader()
			{
			}

			virtual bool LoadStaticModel(const std::string& filename,
										 std::vector<Vertex>& out_vertices,
										 std::vector<uint32_t>& out_indices,
										 NormalGenerationOptions options) override;

			virtual bool LoadAnimatedModel(const std::string& filename,
										   std::vector<Vertex>& out_vertices,
										   std::vector<uint32_t>& out_indices,
										   NormalGenerationOptions options) override;
		};
	}
}

#endif
