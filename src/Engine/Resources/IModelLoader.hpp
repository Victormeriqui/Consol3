#ifndef IMODELLOADER_HPP
#define IMODELLOADER_HPP

#include "Engine/Rendering/Animation.h"
#include "Engine/Rendering/Vertex.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace Engine
{
	namespace Resources
	{
		using namespace Rendering;

		enum class GenerationCondition
		{
			NEVER,
			IF_MISSING,
			ALWAYS
		};

		struct ModelLoadingOptions
		{
			GenerationCondition normal_options;
			GenerationCondition tangent_options;

			ModelLoadingOptions() : normal_options(GenerationCondition::IF_MISSING), tangent_options(GenerationCondition::IF_MISSING)
			{
			}
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
													   ModelLoadingOptions options) = 0;

			[[nodiscard]] virtual bool LoadAnimatedModel(const std::string& filename,
														 std::vector<Frame>& out_frames,
														 std::vector<uint32_t>& out_indices,
														 std::map<std::string, Animation>& out_animations,
														 ModelLoadingOptions options) = 0;
		};
	}
}

#endif
