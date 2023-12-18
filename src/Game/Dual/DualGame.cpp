#include "DualGame.hpp"

#include "../Voxel/VoxelSimulation.hpp"
#include "../Voxel/VoxelUtil.hpp"
#include "Display/RGBColor.hpp"
#include "Math/Util/MathUtil.hpp"

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

        DualGame::DualGame(std::shared_ptr<IFrameDrawer> frame_drawer, std::shared_ptr<IInputManager> input_manager) :
            input_manager(input_manager),
            resource_manager(std::make_shared<ResourceManager>()),
            lighting_system(std::make_shared<LightingSystem>()),
            camera(std::make_shared<Camera>(frame_drawer->GetFrameBufferWidth(), frame_drawer->GetFrameBufferHeight(), 0.001f, 100.0f, 90.0f)),
            voxel_grid(std::make_shared<VoxelGrid>()),
            voxel_scene_renderer(frame_drawer, lighting_system, camera, voxel_grid),
            raster_scene_renderer(frame_drawer, resource_manager, lighting_system, camera)
        {
            LoadResources();

            camera->SetPosition(Vector3(1.0f, -45.9f, -3.0f));

            lighting_system->SetAmbientLightColor(RGBColor(70, 70, 70));

            dir_light = std::make_shared<DirectionalLight>(Vector3(-1, -1, -0.5f));
            dir_light->SetColor(RGBColor(255, 255, 255));
            // lighting_system->AddLight(dir_light);

            point_light = std::make_shared<PointLight>(Vector3(-2, 0, 0));
            point_light->SetRange(300.0f);
            point_light->SetColor(RGBColor(255, 255, 255));
            // lighting_system->AddLight(point_light);

            spot_light = std::make_shared<SpotLight>(Vector3(0, -48, 4), Vector3(0, -0.5f, -1));
            spot_light->SetRange(300.0f);
            spot_light->SetAngle(20.0f);
            spot_light->SetColor(RGBColor(255, 255, 255));
            lighting_system->AddLight(spot_light);

            floor = StaticMesh();
            floor.SetModelResource("plane50").SetTextureResource("../res/tiles.bmp").SetScale(Vector3(12, 12, 12)).SetPosition(Vector3(-6, -42, -6));

            penguin = AnimatedMesh();
            penguin.SetModelResource("../res/penguin.md2")
                .SetTextureResource("../res/bricks.bmp")
                .SetPosition(Vector3(1.0f, -40.9f, 0.0f))
                .SetScale(Vector3(0.05f, 0.05f, 0.05f))
                .SetRotation(Angle(-1.5708f, 0.0f, 0.0f))
                .SetColor(RGBColor(255, 255, 255));
        }

        void DualGame::LoadResources()
        {
            resource_manager->LoadModel("plane50", model_generator.GeneratePlane(50, 50, 0.0f));

            ModelLoadingOptions model_options;
            resource_manager->LoadModel("../res/penguin.md2", model_options);
            resource_manager->LoadTexture("../res/tiles.bmp", TextureLoadingOptions::DEFAULT);
            resource_manager->LoadTexture("../res/bricks.bmp", TextureLoadingOptions::FLIP_Y);
            resource_manager->LoadTexture("../res/bricks_norm.bmp", TextureLoadingOptions::DEFAULT);

            for (int z = VOXEL_GRID_BACKWARDS; z < VOXEL_GRID_FORWARDS - 1; z++)
            {
                for (int x = VOXEL_GRID_LEFT; x < VOXEL_GRID_RIGHT - 1; x++)
                {
                    voxel_grid->SetVoxelData(Vector3I(x, VOXEL_GRID_DOWN, z), { VoxelElement::STONE, 0 });
                }
            }
        }

        void DualGame::Update()
        {
            update_tick++;

            VoxelSimulation::UpdateSimulationDownTop(voxel_grid, update_tick);
            VoxelSimulation::UpdateSimulationTopDown(voxel_grid, update_tick);
        }

        void DualGame::HandleInput()
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

            if (input_manager->IsKeyHeld(Key::P))
                penguin.PlayAnimation("taunt", 1.0f);
        }

        std::chrono::milliseconds DualGame::Render(int64_t delta)
        {
            auto time = std::chrono::high_resolution_clock::now();

            // we have to manually clear the depth buffers as the scene renderers share them and wont clear it themselves
            camera->ClearDepthBuffer();
            lighting_system->ClearDepthBuffers();

            // render raster components
            raster_scene_renderer.DrawShadedMesh(floor);
            raster_scene_renderer.DrawShadedMesh(penguin);
            raster_scene_renderer.RenderSceneShared(delta);

            // render voxels
            cursor_grid_pos     = voxel_grid->GetGridPosition(camera->GetPosition() + (camera->GetLookDirection() * cursor_depth));
            bool cursor_was_set = false;

            if (voxel_grid->IsPositionInsideGrid(cursor_grid_pos))
            {
                cursor_voxel_data = voxel_grid->GetVoxelData(cursor_grid_pos);
                voxel_grid->SetVoxelData(cursor_grid_pos, { VoxelElement::CURSOR, 0 });
                cursor_was_set = true;
            }

            voxel_scene_renderer.RenderSceneShared(delta);

            // reset the cursor to it's previous voxel data so the simulation doesn't include the CURSOR type
            if (cursor_was_set)
                voxel_grid->SetVoxelData(cursor_grid_pos, cursor_voxel_data);

            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time);
        }
    }
}