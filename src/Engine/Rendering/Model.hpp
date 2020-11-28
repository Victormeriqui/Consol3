#ifndef MODEL_HPP
#define MODEL_HPP

#include "Vertex.hpp"
#include "Transform.hpp"
#include "Rasterizer.hpp"
#include "../../Display/HSVColor.hpp"
#include "Texture.hpp"
#include "DepthBuffer.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <memory>


namespace Engine
{
	namespace Rendering
	{
		class Model
		{
		private:
			std::vector<Vertex> vertices;
			std::vector<uint32_t> indices;

			inline std::vector<std::string> SplitString(std::string string, char delimiter) const;

			void CalculateNormals();

		public:
			Model();
			Model(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
			Model(std::string filename);

			[[nodiscard]] std::vector<Vertex> GetVertices() const;
			[[nodiscard]] std::vector<uint32_t> GetIndices() const;

			void DrawModel(const Transform& transform, DepthBuffer& depthbuffer, Rasterizer& rasterizer, const HSVColor& color) const;
			void DrawModel(const Transform& transform, DepthBuffer& depthbuffer, Rasterizer& rasterizer, std::shared_ptr<Texture> texture, const HSVColor& color) const;
		};
	}
}

#endif