#include "SceneRenderer.hpp"

namespace Engine
{
	namespace Rendering
	{
		SceneRenderer::SceneRenderer(std::shared_ptr<IRenderer> renderer, std::shared_ptr<LightingSystem> lighting_system) :
			renderer(std::move(renderer)),
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
				{
					shadowmap_rasterizer.SetModelMatrix(mesh.get().GetTransform());
					shadowmap_rasterizer.DrawVertexBuffer(light->GetLightDepthBuffer().value(),
														  mesh.get().GetModel().GetVertexBuffer(),
														  nocolor,
														  shader_depthmap);
				}

				for (std::reference_wrapper<const AbstractMesh> mesh : render_buffer_shaded)
				{
					shadowmap_rasterizer.SetModelMatrix(mesh.get().GetTransform());
					shadowmap_rasterizer.DrawVertexBuffer(light->GetLightDepthBuffer().value(),
														  mesh.get().GetModel().GetVertexBuffer(),
														  nocolor,
														  shader_depthmap);
				}

				for (std::reference_wrapper<const AbstractMesh> mesh : render_buffer_textured)
				{
					shadowmap_rasterizer.SetModelMatrix(mesh.get().GetTransform());
					shadowmap_rasterizer.DrawVertexBuffer(light->GetLightDepthBuffer().value(),
														  mesh.get().GetModel().GetVertexBuffer(),
														  nocolor,
														  shader_depthmap);
				}

				for (std::reference_wrapper<const AbstractMesh> mesh : render_buffer_shaded_textured)
				{
					shadowmap_rasterizer.SetModelMatrix(mesh.get().GetTransform());
					shadowmap_rasterizer.DrawVertexBuffer(light->GetLightDepthBuffer().value(),
														  mesh.get().GetModel().GetVertexBuffer(),
														  nocolor,
														  shader_depthmap);
				}
			}
		}

		void SceneRenderer::RenderScene()
		{
			camera->ClearDepthBuffer();
			lighting_system->ClearDepthBuffers();

			RenderShadowMapPass();

			rasterizer.SetViewMatrix(camera->GetViewMatrix());

			for (std::reference_wrapper<const AbstractMesh> mesh : render_buffer_plain)
			{
				rasterizer.SetModelMatrix(mesh.get().GetTransform());
				rasterizer.DrawVertexBuffer(camera->GetDepthBuffer(),
											mesh.get().GetModel().GetVertexBuffer(),
											mesh.get().GetColor(),
											shader_plaincolor);
			}

			for (std::reference_wrapper<const AbstractMesh> mesh : render_buffer_shaded)
			{
				rasterizer.SetModelMatrix(mesh.get().GetTransform());
				rasterizer.DrawVertexBuffer(camera->GetDepthBuffer(),
											mesh.get().GetModel().GetVertexBuffer(),
											mesh.get().GetColor(),
											shader_shadedcolor);
			}

			for (std::reference_wrapper<const AbstractMesh> mesh : render_buffer_textured)
			{
				rasterizer.SetModelMatrix(mesh.get().GetTransform());

				// TODO: we need a texture cache to avoid doing this every frame!
				shader_plaintexture.SetTexture(std::make_shared<Texture>(mesh.get().GetTexture()));
				rasterizer.DrawVertexBuffer(camera->GetDepthBuffer(),
											mesh.get().GetModel().GetVertexBuffer(),
											mesh.get().GetColor(),
											shader_plaintexture);
			}

			for (std::reference_wrapper<const AbstractMesh> mesh : render_buffer_shaded_textured)
			{
				rasterizer.SetModelMatrix(mesh.get().GetTransform());

				// TODO: we need a texture cache to avoid doing this every frame!
				shader_plaintexture.SetTexture(std::make_shared<Texture>(mesh.get().GetTexture()));
				rasterizer.DrawVertexBuffer(camera->GetDepthBuffer(),
											mesh.get().GetModel().GetVertexBuffer(),
											mesh.get().GetColor(),
											shader_shadedtexture);
			}

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
