#ifndef SCENERENDERER_HPP
#define SCENERENDERER_HPP

#include "../../Display/IRenderer.hpp"
#include "../../Display/NullRenderer.hpp"
#include "../Resources/ResourceManager.hpp"
#include "AbstractMesh.hpp"
#include "Camera.hpp"
#include "Lighting/LightingSystem.hpp"
#include "Rasterizer.hpp"
#include "Shaders/DepthMapShader.hpp"
#include "Shaders/PlainColorShader.hpp"
#include "Shaders/PlainTextureShader.hpp"
#include "Shaders/ShadedColorShader.hpp"
#include "Shaders/ShadedTextureShader.hpp"

#include <functional>
#include <list>
#include <memory>

namespace Engine
{
	namespace Rendering
	{
		using namespace Display;
		using namespace Resources;

		class SceneRenderer
		{
		private:
			std::shared_ptr<IRenderer> renderer;
			// a non functioning renderer for the shadowmap rasterizer
			std::shared_ptr<NullRenderer> null_renderer;

			Rasterizer rasterizer;
			// a secondary rasterizer that writes to the light's depthbuffer, this is prefered so the main rasterizer doesn't need to be affected
			Rasterizer shadowmap_rasterizer;

			std::shared_ptr<ResourceManager> resource_manager;

			std::shared_ptr<LightingSystem> lighting_system;
			std::shared_ptr<Camera> camera;

			std::list<std::reference_wrapper<const AbstractMesh>> render_buffer_plain;
			std::list<std::reference_wrapper<const AbstractMesh>> render_buffer_shaded;
			std::list<std::reference_wrapper<const AbstractMesh>> render_buffer_textured;
			std::list<std::reference_wrapper<const AbstractMesh>> render_buffer_shaded_textured;

			PlainColorShader shader_plaincolor;
			PlainTextureShader shader_plaintexture;
			ShadedColorShader shader_shadedcolor;
			ShadedTextureShader shader_shadedtexture;
			DepthMapShader shader_depthmap;

			void RenderShadowMapPass();

		public:
			SceneRenderer(std::shared_ptr<IRenderer> renderer,
						  std::shared_ptr<ResourceManager> resource_manager,
						  std::shared_ptr<LightingSystem> lighting_system);

			void SetCamera(std::shared_ptr<Camera> camera);

			void DrawMesh(const AbstractMesh& mesh);
			void DrawShadedMesh(const AbstractMesh& mesh);
			void DrawPixel(uint16_t x, uint16_t y, const HSVColor& color);

			void RenderScene();
		};
	}
}

#endif
