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

        VoxelGame::VoxelGame(std::shared_ptr<IInputManager> input_manager) :
            input_manager(input_manager),
            resource_manager(std::make_shared<ResourceManager>()),
            lighting_system(std::make_shared<LightingSystem>()),
            camera(std::make_shared<Camera>(200, 200, 0.001f, 100.0f, 90.0f)),
            voxel_grid(std::make_shared<VoxelGrid>()),
            voxel_sim(voxel_grid),
            scene_renderer(lighting_system, camera, voxel_grid)
        {
            LoadResources();

            camera->SetPosition(Vector3(-2, -48, 1));
            camera->SetRotation(Angle(0, -200, 0));

            lighting_system->SetAmbientLightColor(RGBColor(100, 100, 100));

            dir_light = std::make_shared<DirectionalLight>(Vector3(-1, -1, -1));
            dir_light->SetColor(RGBColor(255, 255, 255));
            lighting_system->AddLight(dir_light);

            point_light = std::make_shared<PointLight>(Vector3(-2, 0, 0));
            point_light->SetRange(300.0f);
            point_light->SetColor(RGBColor(255, 255, 255));
            // lighting_system->AddLight(point_light);

            spot_light = std::make_shared<SpotLight>(Vector3(0, -48, 4), Vector3(0, -0.5f, -1));
            spot_light->SetRange(300.0f);
            spot_light->SetAngle(20.0f);
            spot_light->SetColor(RGBColor(255, 0, 0));
            lighting_system->AddLight(spot_light);
        }

        void VoxelGame::SetFrameDrawer(std::shared_ptr<IFrameDrawer> frame_drawer)
        {
            this->frame_drawer = std::move(frame_drawer);

            this->frame_drawer->SetupFrameDrawer();

            camera = std::make_shared<Camera>(this->frame_drawer->GetFrameBufferWidth(), this->frame_drawer->GetFrameBufferHeight(), 0.001f, 100.0f, 90.0f);
            camera->SetPosition(Vector3(-2, -48, 1));
            camera->SetRotation(Angle(0, -200, 0));

            scene_renderer = VoxelSceneRenderer(lighting_system, camera, voxel_grid);
            scene_renderer.SetFrameDrawer(this->frame_drawer);
        };

        void VoxelGame::LoadResources()
        {
            for (int z = VOXEL_GRID_BACKWARDS; z < VOXEL_GRID_FORWARDS - 1; z++)
            {
                for (int x = VOXEL_GRID_LEFT; x < VOXEL_GRID_RIGHT - 1; x++)
                {
                    voxel_grid->SetVoxelData(Vector3I(x, VOXEL_GRID_DOWN, z), {VoxelElement::STONE, 0});
                }
            }

            voxel_grid->SetVoxelData(Vector3I(0, VOXEL_GRID_DOWN + 1, 0), {VoxelElement::SAND, 0});
            voxel_grid->SetVoxelData(Vector3I(VOXEL_GRID_LEFT + 30, VOXEL_GRID_DOWN + 1, VOXEL_GRID_BACKWARDS + 30), {VoxelElement::SAND, 0});
            voxel_grid->SetVoxelData(Vector3I(VOXEL_GRID_LEFT + 30, VOXEL_GRID_DOWN + 2, VOXEL_GRID_BACKWARDS + 30), {VoxelElement::SAND, 0});
            voxel_grid->SetVoxelData(Vector3I(VOXEL_GRID_LEFT + 30, VOXEL_GRID_DOWN + 3, VOXEL_GRID_BACKWARDS + 30), {VoxelElement::SAND, 0});
            voxel_grid->SetVoxelData(Vector3I(VOXEL_GRID_LEFT + 30, VOXEL_GRID_DOWN + 4, VOXEL_GRID_BACKWARDS + 30), {VoxelElement::SAND, 0});
            voxel_grid->SetVoxelData(Vector3I(VOXEL_GRID_LEFT + 30, VOXEL_GRID_DOWN + 5, VOXEL_GRID_BACKWARDS + 30), {VoxelElement::SAND, 0});
            voxel_grid->SetVoxelData(Vector3I(VOXEL_GRID_LEFT + 30, VOXEL_GRID_DOWN + 6, VOXEL_GRID_BACKWARDS + 30), {VoxelElement::SAND, 0});
            voxel_grid->SetVoxelData(Vector3I(VOXEL_GRID_LEFT + 30, VOXEL_GRID_DOWN + 7, VOXEL_GRID_BACKWARDS + 30), {VoxelElement::SAND, 0});
            voxel_grid->SetVoxelData(Vector3I(VOXEL_GRID_LEFT + 30, VOXEL_GRID_DOWN + 9, VOXEL_GRID_BACKWARDS + 30), {VoxelElement::SAND, 0});
            voxel_grid->SetVoxelData(Vector3I(VOXEL_GRID_LEFT + 30, VOXEL_GRID_DOWN + 10, VOXEL_GRID_BACKWARDS + 30), {VoxelElement::SAND, 0});
            voxel_grid->SetVoxelData(Vector3I(VOXEL_GRID_LEFT + 30, VOXEL_GRID_DOWN + 11, VOXEL_GRID_BACKWARDS + 30), {VoxelElement::SAND, 0});
            voxel_grid->SetVoxelData(Vector3I(VOXEL_GRID_LEFT + 30, VOXEL_GRID_DOWN + 12, VOXEL_GRID_BACKWARDS + 30), {VoxelElement::SAND, 0});
            voxel_grid->SetVoxelData(Vector3I(VOXEL_GRID_LEFT + 30, VOXEL_GRID_DOWN + 13, VOXEL_GRID_BACKWARDS + 30), {VoxelElement::SAND, 0});
            voxel_grid->SetVoxelData(Vector3I(VOXEL_GRID_LEFT + 30, VOXEL_GRID_DOWN + 14, VOXEL_GRID_BACKWARDS + 30), {VoxelElement::SAND, 0});
            voxel_grid->SetVoxelData(Vector3I(VOXEL_GRID_LEFT + 30, VOXEL_GRID_DOWN + 15, VOXEL_GRID_BACKWARDS + 30), {VoxelElement::SAND, 0});
            voxel_grid->SetVoxelData(Vector3I(VOXEL_GRID_LEFT + 30, VOXEL_GRID_DOWN + 16, VOXEL_GRID_BACKWARDS + 30), {VoxelElement::SAND, 0});
            voxel_grid->SetVoxelData(Vector3I(VOXEL_GRID_LEFT + 30, VOXEL_GRID_DOWN + 17, VOXEL_GRID_BACKWARDS + 30), {VoxelElement::SAND, 0});
            voxel_grid->SetVoxelData(Vector3I(VOXEL_GRID_LEFT + 30, VOXEL_GRID_DOWN + 18, VOXEL_GRID_BACKWARDS + 30), {VoxelElement::SAND, 0});
            voxel_grid->SetVoxelData(Vector3I(VOXEL_GRID_LEFT + 30, VOXEL_GRID_DOWN + 19, VOXEL_GRID_BACKWARDS + 30), {VoxelElement::SAND, 0});
            voxel_grid->SetVoxelData(Vector3I(VOXEL_GRID_LEFT + 30, VOXEL_GRID_DOWN + 20, VOXEL_GRID_BACKWARDS + 30), {VoxelElement::SAND, 0});
        }

        void VoxelGame::Update()
        {
            update_tick++;

            voxel_sim.UpdateSimulationDownTop(update_tick);
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

            if (input_manager->IsKeyHeld(Key::MOUSE3))
                dir_light->SetDirection(camera->GetLookDirection());
            if (input_manager->IsKeyHeld(Key::MOUSE4))
                point_light->SetPosition(camera->GetPosition());
            if (input_manager->IsKeyHeld(Key::MOUSE5))
            {
                spot_light->SetPosition(camera->GetPosition());
                spot_light->SetDirection(camera->GetLookDirection());
            }

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
                voxel_grid->SetVoxelData(cursor_grid_pos, {VoxelElement::CURSOR, 0});
                cursor_was_set = true;
            }

            scene_renderer.RenderScene(delta);

            // reset the cursor to it's previous voxel data so the simulation doesn't include the CURSOR type
            if (cursor_was_set)
                voxel_grid->SetVoxelData(cursor_grid_pos, cursor_voxel_data);

            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time);
        }

        std::string VoxelGame::GetDesiredWindowTitle() const
        {
            return "Voxel game";
        }
    }
}