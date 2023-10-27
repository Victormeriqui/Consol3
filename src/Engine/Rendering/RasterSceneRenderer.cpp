#include "RasterSceneRenderer.hpp"

#include "AnimatedMesh.hpp"
#include "TextureConstants.hpp"

namespace Engine
{
    namespace Rendering
    {
        RasterSceneRenderer::RasterSceneRenderer(std::shared_ptr<IFrameDrawer> frame_drawer, std::shared_ptr<ResourceManager> resource_manager, std::shared_ptr<LightingSystem> lighting_system, std::shared_ptr<Camera> camera) :
            frame_drawer(std::move(frame_drawer)),
            null_frame_drawer(std::make_shared<NullFrameDrawer>()),
            rasterizer(this->frame_drawer),
            shadowmap_rasterizer(null_frame_drawer),
            resource_manager(std::move(resource_manager)),
            lighting_system(std::move(lighting_system)),
            camera(std::move(camera))
        {
            shader_shaded.SetLightingSystem(this->lighting_system);
            rasterizer.SetProjectionMatrix(this->camera->GetProjectionMatrix());
        }

        void RasterSceneRenderer::DrawMesh(AbstractMesh& mesh)
        {
            render_buffer_plain.push_back(std::reference_wrapper(mesh));

            if (mesh.IsAnimated())
                updatable_animated_meshes.push_back(std::reference_wrapper(dynamic_cast<AnimatedMesh&>(mesh)));
        }

        void RasterSceneRenderer::DrawShadedMesh(AbstractMesh& mesh)
        {
            render_buffer_shaded.push_back(std::reference_wrapper(mesh));

            if (mesh.IsAnimated())
                updatable_animated_meshes.push_back(std::reference_wrapper(dynamic_cast<AnimatedMesh&>(mesh)));
        }

        void RasterSceneRenderer::RenderStaticMesh(Rasterizer& rasterizer, AbstractMesh& mesh, DepthBuffer& depthbuffer, IShader& shader, const RGBColor& color)
        {
            rasterizer.SetModelMatrix(mesh.GetTransform());
            rasterizer.DrawVertexBuffer(depthbuffer, resource_manager->GetLoadedStaticModel(mesh.GetModelResource()).value()->GetVertexBuffer(), color, shader);
        }

        void RasterSceneRenderer::RenderAnimatedMesh(Rasterizer& rasterizer, AbstractMesh& mesh, DepthBuffer& depthbuffer, IShader& shader, const RGBColor& color)
        {
            AnimatedMesh& animated_mesh = dynamic_cast<AnimatedMesh&>(mesh);

            rasterizer.SetModelMatrix(mesh.GetTransform());
            rasterizer.DrawVertexBuffer(depthbuffer,
                                        resource_manager->GetLoadedAnimatedModel(mesh.GetModelResource()).value()->GetVertexBuffer(animated_mesh.GetCurrentAnimationName(), animated_mesh.GetCurrentAnimationProgress()),
                                        color,
                                        shader);

            animated_mesh.UpdateAnimation();
        }

        void RasterSceneRenderer::RenderMesh(Rasterizer& rasterizer, AbstractMesh& mesh, DepthBuffer& depthbuffer, IShader& shader, const RGBColor& color)
        {
            if (mesh.IsAnimated())
                RenderAnimatedMesh(rasterizer, mesh, depthbuffer, shader, color);
            else
                RenderStaticMesh(rasterizer, mesh, depthbuffer, shader, color);
        }

        void RasterSceneRenderer::RenderShadowMapPass()
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

        void RasterSceneRenderer::RenderScene(int64_t delta)
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
                std::optional<std::shared_ptr<Texture>> texture    = resource_manager->GetLoadedTexture(mesh.get().GetTextureResource());
                std::optional<std::shared_ptr<Texture>> normal_map = resource_manager->GetLoadedTexture(mesh.get().GetNormalMapResource());

                shader_shaded.SetTexture(texture.value_or(TextureConstants::White()));
                shader_shaded.SetCameraPosition(camera->GetPosition());
                shader_shaded.SetMaterialProperties(mesh.get().GetMaterialProperties());
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

        void RasterSceneRenderer::DrawPixel(uint16_t x, uint16_t y, const RGBColor& color)
        {
            rasterizer.DrawPixel(x, y, color);
        }
    }
}
