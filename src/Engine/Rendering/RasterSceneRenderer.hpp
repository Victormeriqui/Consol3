#ifndef RASTERSCENERENDERER_HPP
#define RASTERSCENERENDERER_HPP

#include "AbstractMesh.hpp"
#include "AnimatedMesh.hpp"
#include "Camera.hpp"
#include "Display/IFrameDrawer.hpp"
#include "Display/NullFrameDrawer.hpp"
#include "Display/RGBColor.hpp"
#include "Engine/Resources/ResourceManager.hpp"
#include "Lighting/LightingSystem.hpp"
#include "Rasterizer.hpp"
#include "Shaders/DepthMapShader.hpp"
#include "Shaders/PlainShader.hpp"
#include "Shaders/ShadedShader.hpp"

#include <functional>
#include <list>
#include <memory>

namespace Engine
{
    namespace Rendering
    {
        using namespace Display;
        using namespace Resources;
        using namespace Lighting;

        class RasterSceneRenderer
        {
        private:
            std::shared_ptr<IFrameDrawer> frame_drawer;
            // a non functioning frame drawer for the shadowmap rasterizer
            std::shared_ptr<NullFrameDrawer> null_frame_drawer;

            Rasterizer rasterizer;
            // a secondary rasterizer that writes to the light's depthbuffer, this is prefered so the main rasterizer doesn't need to be affected
            Rasterizer shadowmap_rasterizer;

            std::shared_ptr<ResourceManager> resource_manager;

            std::shared_ptr<LightingSystem> lighting_system;
            std::shared_ptr<Camera> camera;

            std::list<std::reference_wrapper<AbstractMesh>> render_buffer_plain;
            std::list<std::reference_wrapper<AbstractMesh>> render_buffer_shaded;

            std::list<std::reference_wrapper<AnimatedMesh>> updatable_animated_meshes;

            PlainShader shader_plain;
            ShadedShader shader_shaded;
            DepthMapShader shader_depthmap;

            void RenderShadowMapPass();
            void RenderStaticMesh(Rasterizer& rasterizer, AbstractMesh& mesh, DepthBuffer& depthbuffer, IShader& shader, const RGBColor& color);
            void RenderAnimatedMesh(Rasterizer& rasterizer, AbstractMesh& mesh, DepthBuffer& depthbuffer, IShader& shader, const RGBColor& color);

            void RenderMesh(Rasterizer& rasterizer, AbstractMesh& mesh, DepthBuffer& depthbuffer, IShader& shader, const RGBColor& color);

        public:
            RasterSceneRenderer(std::shared_ptr<IFrameDrawer> frame_drawer, std::shared_ptr<ResourceManager> resource_manager, std::shared_ptr<LightingSystem> lighting_system, std::shared_ptr<Camera> camera);
            void DrawMesh(AbstractMesh& mesh);
            void DrawShadedMesh(AbstractMesh& mesh);
            void DrawPixel(uint16_t x, uint16_t y, const RGBColor& color);

            void RenderScene(int64_t delta);
        };
    }
}

#endif
