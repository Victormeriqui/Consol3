#include "SceneRenderer.hpp"

#include "AnimatedMesh.hpp"
#include "TextureConstants.hpp"

namespace Engine
{
	namespace Rendering
	{
		SceneRenderer::SceneRenderer(std::shared_ptr<IRenderer> renderer,
									 std::shared_ptr<ResourceManager> resource_manager,
									 std::shared_ptr<LightingSystem> lighting_system) :
			renderer(std::move(renderer)),
			resource_manager(std::move(resource_manager)),
			null_renderer(std::make_shared<NullRenderer>()),
			rasterizer(this->renderer),
			shadowmap_rasterizer(null_renderer),
			lighting_system(std::move(lighting_system)),
			camera(std::make_shared<Camera>())
		{
			shader_shaded.SetLightingSystem(this->lighting_system);
		}

		void SceneRenderer::SetCamera(std::shared_ptr<Camera> camera)
		{
			this->camera = std::move(camera);

			rasterizer.SetProjectionMatrix(this->camera->GetProjectionMatrix());
		}

		void SceneRenderer::DrawMesh(AbstractMesh& mesh)
		{
			render_buffer_plain.push_back(std::reference_wrapper(mesh));

			if (mesh.IsAnimated())
				updatable_animated_meshes.push_back(std::reference_wrapper(dynamic_cast<AnimatedMesh&>(mesh)));
		}

		void SceneRenderer::DrawShadedMesh(AbstractMesh& mesh)
		{
			render_buffer_shaded.push_back(std::reference_wrapper(mesh));

			if (mesh.IsAnimated())
				updatable_animated_meshes.push_back(std::reference_wrapper(dynamic_cast<AnimatedMesh&>(mesh)));
		}

		void SceneRenderer::RenderStaticMesh(
			Rasterizer& rasterizer, AbstractMesh& mesh, DepthBuffer& depthbuffer, IShader& shader, const RGBColor& color)
		{
			rasterizer.SetModelMatrix(mesh.GetTransform());
			rasterizer.DrawVertexBuffer(depthbuffer,
										resource_manager->GetLoadedStaticModel(mesh.GetModelResource()).value()->GetVertexBuffer(),
										color,
										shader);
		}

		void SceneRenderer::RenderAnimatedMesh(
			Rasterizer& rasterizer, AbstractMesh& mesh, DepthBuffer& depthbuffer, IShader& shader, const RGBColor& color)
		{
			AnimatedMesh& animated_mesh = dynamic_cast<AnimatedMesh&>(mesh);

			rasterizer.SetModelMatrix(mesh.GetTransform());
			rasterizer.DrawVertexBuffer(depthbuffer,
										resource_manager->GetLoadedAnimatedModel(mesh.GetModelResource())
											.value()
											->GetVertexBuffer(animated_mesh.GetCurrentAnimationName(), animated_mesh.GetCurrentAnimationProgress()),
										color,
										shader);

			animated_mesh.UpdateAnimation();
		}

		void SceneRenderer::RenderMesh(Rasterizer& rasterizer, AbstractMesh& mesh, DepthBuffer& depthbuffer, IShader& shader, const RGBColor& color)
		{
			if (mesh.IsAnimated())
				RenderAnimatedMesh(rasterizer, mesh, depthbuffer, shader, color);
			else
				RenderStaticMesh(rasterizer, mesh, depthbuffer, shader, color);
		}

		void SceneRenderer::RenderShadowMapPass()
		{
			for (std::shared_ptr<ILight> light : lighting_system->GetLights())
			{
				if (!light->IsShadowCaster())
					continue;

				shadowmap_rasterizer.SetProjectionMatrix(light->GetProjectionMatrix().value());
				shadowmap_rasterizer.SetViewMatrix(light->GetViewMatrix().value());

				static const RGBColor nocolor;

				for (std::reference_wrapper<AbstractMesh> mesh : render_buffer_plain)
					RenderMesh(shadowmap_rasterizer, mesh.get(), light->GetLightDepthBuffer().value(), shader_depthmap, nocolor);

				for (std::reference_wrapper<AbstractMesh> mesh : render_buffer_shaded)
					RenderMesh(shadowmap_rasterizer, mesh.get(), light->GetLightDepthBuffer().value(), shader_depthmap, nocolor);
			}
		}

		void SceneRenderer::RenderScene(int64_t delta)
		{
			camera->ClearDepthBuffer();
			lighting_system->ClearDepthBuffers();

			RenderShadowMapPass();

			rasterizer.SetViewMatrix(camera->GetViewMatrix());

			for (std::reference_wrapper<AbstractMesh> mesh : render_buffer_plain)
			{
				std::optional<std::shared_ptr<Texture>> texture = resource_manager->GetLoadedTexture(mesh.get().GetTextureResource());

				shader_plain.SetTexture(texture.value_or(TextureConstants::White()));

				RenderMesh(rasterizer, mesh.get(), camera->GetDepthBuffer(), shader_plain, mesh.get().GetColor());
			}

			for (std::reference_wrapper<AbstractMesh> mesh : render_buffer_shaded)
			{
				std::optional<std::shared_ptr<Texture>> texture	   = resource_manager->GetLoadedTexture(mesh.get().GetTextureResource());
				std::optional<std::shared_ptr<Texture>> normal_map = resource_manager->GetLoadedTexture(mesh.get().GetNormalMapResource());

				shader_shaded.SetTexture(texture.value_or(TextureConstants::White()));

				if (normal_map.has_value())
					shader_shaded.SetNormalMap(normal_map.value());
				else
					shader_shaded.DisableNormalMap();

				RenderMesh(rasterizer, mesh.get(), camera->GetDepthBuffer(), shader_shaded, mesh.get().GetColor());
			}

			for (std::reference_wrapper<AnimatedMesh> mesh : updatable_animated_meshes)
			{
				mesh.get().UpdateAnimation();
			}

			render_buffer_plain.clear();
			render_buffer_shaded.clear();
			updatable_animated_meshes.clear();
		}

		void SceneRenderer::DrawPixel(uint16_t x, uint16_t y, const RGBColor& color)
		{
			rasterizer.DrawPixel(x, y, color);
		}
	}
}
