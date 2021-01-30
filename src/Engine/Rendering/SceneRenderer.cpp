#include "SceneRenderer.hpp"

#include "AnimatedMesh.hpp"

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
			shader_shadedcolor.SetLightingSystem(this->lighting_system);
			shader_shadedtexture.SetLightingSystem(this->lighting_system);
		}

		void SceneRenderer::SetCamera(std::shared_ptr<Camera> camera)
		{
			this->camera = std::move(camera);

			rasterizer.SetProjectionMatrix(this->camera->GetProjectionMatrix());
		}

		void SceneRenderer::DrawMesh(const AbstractMesh& mesh)
		{
			if (mesh.IsTextured())
				render_buffer_textured.push_back(std::reference_wrapper(mesh));
			else
				render_buffer_plain.push_back(std::reference_wrapper(mesh));
		}

		void SceneRenderer::DrawShadedMesh(const AbstractMesh& mesh)
		{
			if (mesh.IsTextured())
				render_buffer_shaded_textured.push_back(std::reference_wrapper(mesh));
			else
				render_buffer_shaded.push_back(std::reference_wrapper(mesh));
		}

		void SceneRenderer::RenderStaticMesh(
			Rasterizer& rasterizer, const AbstractMesh& mesh, DepthBuffer& depthbuffer, IShader& shader, const HSVColor& color)
		{
			rasterizer.SetModelMatrix(mesh.GetTransform());
			rasterizer.DrawVertexBuffer(depthbuffer,
										resource_manager->GetLoadedStaticModel(mesh.GetModelResource())->GetVertexBuffer(),
										color,
										shader);
		}

		void SceneRenderer::RenderAnimatedMesh(
			Rasterizer& rasterizer, const AbstractMesh& mesh, DepthBuffer& depthbuffer, IShader& shader, const HSVColor& color)
		{
			const AnimatedMesh& animated_mesh = dynamic_cast<const AnimatedMesh&>(mesh);

			rasterizer.SetModelMatrix(mesh.GetTransform());
			rasterizer.DrawVertexBuffer(depthbuffer,
										resource_manager->GetLoadedStaticModel(mesh.GetModelResource())->GetVertexBuffer(),
										color,
										shader);
		}

		void SceneRenderer::RenderShadowMapPass()
		{
			for (std::shared_ptr<ILight> light : lighting_system->GetLights())
			{
				if (!light->IsShadowCaster())
					continue;

				shadowmap_rasterizer.SetProjectionMatrix(light->GetProjectionMatrix().value());
				shadowmap_rasterizer.SetViewMatrix(light->GetViewMatrix().value());

				HSVColor nocolor;

				for (std::reference_wrapper<const AbstractMesh> mesh : render_buffer_plain)
					RenderStaticMesh(shadowmap_rasterizer, mesh.get(), light->GetLightDepthBuffer().value(), shader_depthmap, nocolor);

				for (std::reference_wrapper<const AbstractMesh> mesh : render_buffer_shaded)
					RenderStaticMesh(shadowmap_rasterizer, mesh.get(), light->GetLightDepthBuffer().value(), shader_depthmap, nocolor);

				for (std::reference_wrapper<const AbstractMesh> mesh : render_buffer_textured)
					RenderStaticMesh(shadowmap_rasterizer, mesh.get(), light->GetLightDepthBuffer().value(), shader_depthmap, nocolor);

				for (std::reference_wrapper<const AbstractMesh> mesh : render_buffer_shaded_textured)
					RenderStaticMesh(shadowmap_rasterizer, mesh.get(), light->GetLightDepthBuffer().value(), shader_depthmap, nocolor);
			}
		}

		void SceneRenderer::RenderScene()
		{
			camera->ClearDepthBuffer();
			lighting_system->ClearDepthBuffers();

			RenderShadowMapPass();

			rasterizer.SetViewMatrix(camera->GetViewMatrix());

			for (std::reference_wrapper<const AbstractMesh> mesh : render_buffer_plain)
				RenderStaticMesh(rasterizer, mesh.get(), camera->GetDepthBuffer(), shader_plaincolor, mesh.get().GetColor());

			for (std::reference_wrapper<const AbstractMesh> mesh : render_buffer_shaded)
				RenderStaticMesh(rasterizer, mesh.get(), camera->GetDepthBuffer(), shader_shadedcolor, mesh.get().GetColor());

			for (std::reference_wrapper<const AbstractMesh> mesh : render_buffer_textured)
				RenderStaticMesh(rasterizer, mesh.get(), camera->GetDepthBuffer(), shader_plaintexture, mesh.get().GetColor());

			for (std::reference_wrapper<const AbstractMesh> mesh : render_buffer_shaded_textured)
				RenderStaticMesh(rasterizer, mesh.get(), camera->GetDepthBuffer(), shader_shadedtexture, mesh.get().GetColor());

			render_buffer_plain.clear();
			render_buffer_shaded.clear();
			render_buffer_textured.clear();
			render_buffer_shaded_textured.clear();
		}

		void SceneRenderer::DrawPixel(uint16_t x, uint16_t y, const HSVColor& color)
		{
			rasterizer.DrawPixel(x, y, color);
		}
	}
}
