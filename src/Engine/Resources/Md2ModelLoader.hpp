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
		private:
			inline std::vector<std::string> SplitString(std::string string, char delimiter) const;
			inline uint32_t CountElements(std::ifstream& file_stream, const std::string& startswith) const;

			void CalculateNormals(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

		public:
			Md2ModelLoader()
			{
			}

			virtual bool LoadStaticModel(const std::string& filename,
										 std::vector<Vertex>& out_vertices,
										 std::vector<uint32_t>& out_indices,
										 NormalGenerationOptions options) override;
		};
	}
}

#endif
