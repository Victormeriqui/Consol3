#ifndef VOXELGAME_HPP
#define VOXELGAME_HPP

#include "IGame.hpp"

#include "Engine/Input/IInputManager.hpp"
#include "Engine/Rendering/Camera.hpp"
#include "Engine/Rendering/VoxelGrid.hpp"
#include "Engine/Rendering/VoxelSceneRenderer.hpp"
#include "Engine/Resources/ResourceManager.hpp"
#include "VoxelTypes.hpp"

#include <chrono>
#include <cstdint>
#include <memory>
#include <random>

namespace Game
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

        VoxelType GetVoxel(uint16_t x, uint16_t y, uint16_t z) const;
        void SetVoxel(uint16_t x, uint16_t y, uint16_t z, VoxelType voxel);

        VoxelType GetVoxel(const Vector3& pos) const;
        void SetVoxel(const Vector3& pos, VoxelType voxel);

    public:
        VoxelGame(std::shared_ptr<IFrameDrawer> frame_drawer, std::shared_ptr<IInputManager> input_manager);

        virtual void HandleInput() override;
        virtual void Update() override;
        virtual std::chrono::milliseconds Render(int64_t delta) override;
    };
}

#endif
