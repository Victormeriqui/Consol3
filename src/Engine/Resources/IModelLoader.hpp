#ifndef IMODELLOADER_HPP
#define IMODELLOADER_HPP

#include "../Rendering/Animation.h"
#include "../Rendering/Vertex.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace Engine
{
	namespace Resources
	{
		using namespace Rendering;

		enum class NormalGenerationOptions
		{
			GENERATE_DISABLED,
			GENERATE_IF_MISSING,
			GENERATE_FORCED
		};

		class IModelLoader
		{
		protected:
			IModelLoader()
			{
			}

		public:
			[[nodiscard]] virtual bool LoadStaticModel(const std::string& filename,
													   std::vector<Vertex>& out_vertices,
													   std::vector<uint32_t>& out_indices,
													   NormalGenerationOptions options) = 0;

			[[nodiscard]] virtual bool LoadAnimatedModel(const std::string& filename,
														 std::vector<Frame>& out_frames,
														 std::vector<uint32_t>& out_indices,
														 std::map<std::string, Animation>& out_animations,
														 NormalGenerationOptions options) = 0;
		};
	}
}

#endif
