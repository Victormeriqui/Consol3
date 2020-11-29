#ifndef MODEL_HPP
#define MODEL_HPP

#include "Vertex.hpp"
#include "Transform.hpp"
#include "Rasterizer.hpp"
#include "../../Display/HSVColor.hpp"
#include "Texture.hpp"
#include "DepthBuffer.hpp"
#include "VertexBuffer.hpp"
#include "Shaders/ShaderStore.hpp"
#include "Lighting/LightingSystem.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

namespace Engine
{
	namespace Rendering
	{
		using namespace Shaders;
		using namespace Lighting;

		static ShaderStore shader_store;

		class Model
		{
		private:
			VertexBuffer vertex_buffer;

			inline std::vector<std::string> SplitString(std::string string, char delimiter) const;

			void CalculateNormals(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

		public:
			Model();
			Model(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
			Model(std::string filename);

			[[nodiscard]] const VertexBuffer& GetVertexBuffer() const;

			void DrawModel(const Transform& transform, DepthBuffer& depthbuffer, Rasterizer& rasterizer, const HSVColor& color) const;
			void DrawShadedModel(const Transform& transform, DepthBuffer& depthbuffer, Rasterizer& rasterizer, const LightingSystem& lighting_system, const HSVColor& color) const;
			void DrawTexturedModel(const Transform& transform, DepthBuffer& depthbuffer, Rasterizer& rasterizer, std::shared_ptr<Texture> texture, const HSVColor& color) const;
			void DrawTexturedAndShadedModel(const Transform& transform, DepthBuffer& depthbuffer, Rasterizer& rasterizer, const LightingSystem& lighting_system, std::shared_ptr<Texture> texture,
				const HSVColor& color) const;
		};
	}
}

#endif