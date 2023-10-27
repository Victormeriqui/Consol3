
#ifndef VOXELSCENERENDERER_HPP
#define VOXELSCENERENDERER_HPP

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

        class VoxelSceneRenderer
        {
        private:
            std::shared_ptr<IFrameDrawer> frame_drawer;

            std::shared_ptr<ResourceManager> resource_manager;

            std::shared_ptr<Camera> camera;

        public:
            VoxelSceneRenderer(std::shared_ptr<IFrameDrawer> frame_drawer, std::shared_ptr<ResourceManager> resource_manager, std::shared_ptr<Camera> camera);

            void DrawPixel(uint16_t x, uint16_t y, const RGBColor& color);

            void RenderScene(int64_t delta);
        };
    }
}

#endif
