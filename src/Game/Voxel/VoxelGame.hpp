#ifndef VOXELGAME_HPP
#define VOXELGAME_HPP

#include "IGame.hpp"

#include "Engine/Input/IInputManager.hpp"
#include "Engine/Rendering/Camera.hpp"
#include "Engine/Rendering/VoxelSceneRenderer.hpp"
#include "Engine/Resources/ResourceManager.hpp"
#include "Engine/VoxelGrid.hpp"
#include "Engine/VoxelTypes.hpp"

#include <chrono>
#include <cstdint>
#include <memory>
#include <random>

namespace Game
{
    namespace Voxel
    {
        using namespace Display;
        using namespace Engine;
        using namespace Engine::Rendering;
        using namespace Math;
        using namespace Resources;
        using namespace Input;

        class VoxelGame : public IGame
        {
        private:
            std::shared_ptr<IInputManager> input_manager;

            std::shared_ptr<ResourceManager> resource_manager;
            std::shared_ptr<Camera> camera;

            std::shared_ptr<VoxelGrid<VOXEL_GRID_WIDTH, VOXEL_GRID_HEIGHT, VOXEL_GRID_DEPTH>> voxel_grid;
            VoxelSceneRenderer scene_renderer;

            virtual void LoadResources() override;

            float mov_speed = 0.05f;
            bool shifting   = false;

            float increasing_counter = 0;

            std::mt19937 random_generator;

        public:
            VoxelGame(std::shared_ptr<IFrameDrawer> frame_drawer, std::shared_ptr<IInputManager> input_manager);

            virtual void HandleInput() override;
            virtual void Update() override;
            virtual std::chrono::milliseconds Render(int64_t delta) override;
        };
    }
}
#endif
