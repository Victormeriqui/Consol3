#include "Md2ModelLoader.hpp"
#include "Util/FileUtils.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>

namespace Engine
{
	namespace Resources
	{
		bool Md2ModelLoader::LoadStaticModel(const std::string& filename,
											 std::vector<Vertex>& out_vertices,
											 std::vector<uint32_t>& out_indices,
											 NormalGenerationOptions options)
		{
			// TODO load only one frame's vertices
			return false;
		}

		bool Md2ModelLoader::LoadAnimatedModel(const std::string& filename,
											   std::vector<Vertex>& out_vertices,
											   std::vector<uint32_t>& out_indices,
											   NormalGenerationOptions options)
		{
			// TODO
			return true;
		}
	}
}
