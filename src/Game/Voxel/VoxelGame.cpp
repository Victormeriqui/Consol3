#include "VoxelGame.hpp"

#include "Display/RGBColor.hpp"
#include "Math/Util/MathUtil.hpp"
#include "VoxelSimulation.hpp"
#include "VoxelUtil.hpp"

#include <algorithm>
#include <random>
#include <vector>

using namespace Math;
using namespace Math::Util;

namespace Game
{
    namespace Voxel
    {
        using namespace Display;
        using namespace Engine;
        using namespace Engine::Rendering;
        using namespace Math;

        VoxelGame::VoxelGame(std::shared_ptr<IFrameDrawer> frame_drawer, std::shared_ptr<IInputManager> input_manager) :
            input_manager(input_manager),
            resource_manager(std::make_shared<ResourceManager>()),
            lighting_system(std::make_shared<LightingSystem>()),
            camera(std::make_shared<Camera>(frame_drawer->GetFrameBufferWidth(), frame_drawer->GetFrameBufferHeight(), 0.001f, 100.0f, 90.0f)),
            voxel_grid(std::make_shared<VoxelGrid>()),
            scene_renderer(frame_drawer, resource_manager, lighting_system, camera, voxel_grid)
        {
            LoadResources();

            camera->SetPosition(Vector3(40, 40, 30));
        }

        void VoxelGame::LoadResources()
        {
            for (int z = VOXEL_GRID_BACKWARDS; z < VOXEL_GRID_FORWARDS - 1; z++)
            {
                for (int x = VOXEL_GRID_LEFT; x < VOXEL_GRID_RIGHT - 1; x++)
                {
                    voxel_grid->SetVoxelData(Vector3I(x, 0, z), { VoxelElement::STONE, 0 });
                }
            }
        }

        void VoxelGame::Update()
        {
            update_tick++;

            VoxelSimulation::UpdateSimulationDownTop(voxel_grid, update_tick);
            VoxelSimulation::UpdateSimulationTopDown(voxel_grid, update_tick);
        }

        void VoxelGame::HandleInput()
        {
            // camera
            if (input_manager->IsKeyPressed(Key::CAPITAL))
            {
                Vector2 mouse_dist_center = input_manager->GetMouseDistanceToCenter();

                if (mouse_dist_center.x != 0)
                    camera->RotateYaw(mouse_dist_center.x * 0.1f);
                if (mouse_dist_center.y != 0)
                    camera->RotatePitch(mouse_dist_center.y * 0.1f);

                input_manager->SetMousePositionToCenter();
            }
            if (input_manager->IsKeyHeld(Key::SPACE))
                camera->MoveY(mov_speed);

            if (input_manager->IsKeyHeld(Key::LCONTROL))
                camera->MoveY(-mov_speed);

            if (input_manager->IsKeyHeld(Key::A))
                camera->MoveX(-mov_speed);

            if (input_manager->IsKeyHeld(Key::D))
                camera->MoveX(mov_speed);

            if (input_manager->IsKeyHeld(Key::S))
                camera->MoveZ(-mov_speed);

            if (input_manager->IsKeyHeld(Key::W))
                camera->MoveZ(mov_speed);

            if (input_manager->IsKeyHeld(Key::LSHIFT))
            {
                shifting = true;
                // mov_speed = 0.001f;
                mov_speed = 0.5f;
            }
            else if (shifting)
            {
                shifting  = false;
                mov_speed = 0.05f;
            }

            // game
            if (input_manager->IsKeyHeld(Key::Q))
                cursor_depth -= 0.2f;

            if (input_manager->IsKeyHeld(Key::E))
                cursor_depth += 0.2f;

            if (input_manager->IsKeyHeld(Key::MOUSE2))
                VoxelUtil::SpawnVoxel(voxel_grid, cursor_grid_pos, selected_voxel);

            if (input_manager->IsKeyHeld(Key::N1))
                selected_voxel = VoxelElement::SAND;

            if (input_manager->IsKeyHeld(Key::N2))
                selected_voxel = VoxelElement::WATER;

            if (input_manager->IsKeyHeld(Key::N3))
                selected_voxel = VoxelElement::STEEL;

            if (input_manager->IsKeyHeld(Key::N4))
                selected_voxel = VoxelElement::STEAM;
        }

        std::chrono::milliseconds VoxelGame::Render(int64_t delta)
        {
            auto time = std::chrono::high_resolution_clock::now();

            cursor_grid_pos     = voxel_grid->GetGridPosition(camera->GetPosition() + (camera->GetLookDirection() * cursor_depth));
            bool cursor_was_set = false;

            if (voxel_grid->IsPositionInsideGrid(cursor_grid_pos))
            {
                cursor_voxel_data = voxel_grid->GetVoxelData(cursor_grid_pos);
                voxel_grid->SetVoxelData(cursor_grid_pos, { VoxelElement::CURSOR, 0 });
                cursor_was_set = true;
            }

            scene_renderer.RenderScene(delta);

            // reset the cursor to it's previous voxel data so the simulation doesn't include the CURSOR type
            if (cursor_was_set)
                voxel_grid->SetVoxelData(cursor_grid_pos, cursor_voxel_data);

            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time);
        }
    }
}