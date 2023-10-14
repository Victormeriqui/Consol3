#ifndef OBJMODELLOADER_HPP
#define OBJMODELLOADER_HPP

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

		class ObjModelLoader : public IModelLoader
		{
		public:
			ObjModelLoader()
			{
			}

			virtual bool LoadStaticModel(const std::string& filename,
										 std::vector<Vertex>& out_vertices,
										 std::vector<uint32_t>& out_indices,
										 ModelLoadingOptions options) override;

			virtual bool LoadAnimatedModel(const std::string& filename,
										   std::vector<Frame>& out_frames,
										   std::vector<uint32_t>& out_indices,
										   std::map<std::string, Animation>& out_animations,
										   ModelLoadingOptions options) override;
		};
	}
}

#endif
