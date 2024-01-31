#ifndef DUALGAME_HPP
#define DUALGAME_HPP

#include "IGame.hpp"

#include "../Raster/ModelGenerator.hpp"
#include "../Voxel/VoxelSimulation.hpp"
#include "../Voxel/VoxelUtil.hpp"
#include "Engine/Input/IInputManager.hpp"
#include "Engine/Rendering/AnimatedMesh.hpp"
#include "Engine/Rendering/Camera.hpp"
#include "Engine/Rendering/Lighting/DirectionalLight.hpp"
#include "Engine/Rendering/Lighting/ILight.hpp"
#include "Engine/Rendering/Lighting/LightingSystem.hpp"
#include "Engine/Rendering/Lighting/PointLight.hpp"
#include "Engine/Rendering/Lighting/SpotLight.hpp"
#include "Engine/Rendering/RasterSceneRenderer.hpp"
#include "Engine/Rendering/StaticMesh.hpp"
#include "Engine/Rendering/VoxelSceneRenderer.hpp"
#include "Engine/Resources/ResourceManager.hpp"
#include "Engine/VoxelElements.hpp"
#include "Engine/VoxelGrid.hpp"
#include "Math/Vector3.hpp"

#include <chrono>
#include <cstdint>
#include <memory>
#include <vector>

namespace Game
{
    namespace Voxel
    {
        using namespace Display;
        using namespace Engine;
        using namespace Engine::Rendering;
        using namespace Math;
        using namespace Lighting;
        using namespace Resources;
        using namespace Input;

        class DualGame : public IGame
        {
        private:
            std::shared_ptr<IInputManager> input_manager;

            std::shared_ptr<ResourceManager> resource_manager;
            std::shared_ptr<LightingSystem> lighting_system;
            std::shared_ptr<Camera> camera;

            std::shared_ptr<VoxelGrid> voxel_grid;
            VoxelSimulation voxel_sim;

            VoxelSceneRenderer voxel_scene_renderer;
            RasterSceneRenderer raster_scene_renderer;

            Raster::ModelGenerator model_generator;
            virtual void LoadResources() override;

            std::shared_ptr<DirectionalLight> dir_light;
            std::shared_ptr<PointLight> point_light;
            std::shared_ptr<SpotLight> spot_light;

            AnimatedMesh penguin;
            StaticMesh floor;

            float mov_speed = 0.05f;
            bool shifting   = false;

            Vector3I cursor_center_grid_pos;
            std::vector<VoxelData> prev_cursor_data;
            float cursor_depth = 5.0f;
            float cursor_size  = 1.0f;

            VoxelElement selected_voxel = VoxelElement::SAND;

            uint64_t update_tick = 0;

        public:
            DualGame(std::shared_ptr<IFrameDrawer> frame_drawer, std::shared_ptr<IInputManager> input_manager);

            virtual void HandleInput() override;
            virtual void Update() override;
            virtual std::chrono::milliseconds Render(int64_t delta) override;
            virtual std::string GetDesiredWindowTitle() const override;
        };
    }
}
#endif
