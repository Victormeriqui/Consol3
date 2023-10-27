#include "VoxelSceneRenderer.hpp"

#include "AnimatedMesh.hpp"
#include "TextureConstants.hpp"

namespace Engine
{
    namespace Rendering
    {
        VoxelSceneRenderer::VoxelSceneRenderer(std::shared_ptr<IFrameDrawer> frame_drawer, std::shared_ptr<ResourceManager> resource_manager, std::shared_ptr<Camera> camera) :
            frame_drawer(std::move(frame_drawer)),
            resource_manager(std::move(resource_manager)),
            camera(std::move(camera))
        {
        }

        void VoxelSceneRenderer::RenderScene(int64_t delta)
        {
            camera->ClearDepthBuffer();
        }

        void VoxelSceneRenderer::DrawPixel(uint16_t x, uint16_t y, const RGBColor& color)
        {
        }
    }
}
