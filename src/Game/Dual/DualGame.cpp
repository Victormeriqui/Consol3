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
            voxel_sim(voxel_grid),
            voxel_scene_renderer(frame_drawer, lighting_system, camera, voxel_grid),
            raster_scene_renderer(frame_drawer, resource_manager, lighting_system, camera)
        {
            LoadResources();

            camera->SetPosition(Vector3(1.0f, -45.9f, -3.0f));

            lighting_system->SetAmbientLightColor(RGBColor(150, 150, 150));

            dir_light = std::make_shared<DirectionalLight>(Vector3(-1, -1, -0.5f));
            dir_light->SetColor(RGBColor(255, 255, 255));
            lighting_system->AddLight(dir_light);

            point_light = std::make_shared<PointLight>(Vector3(-2, 0, 0));
            point_light->SetRange(300.0f);
            point_light->SetColor(RGBColor(255, 255, 255));
            // lighting_system->AddLight(point_light);

            spot_light = std::make_shared<SpotLight>(Vector3(0, -48, 4), Vector3(0, -0.5f, -1));
            spot_light->SetRange(300.0f);
            spot_light->SetAngle(20.0f);
            spot_light->SetColor(RGBColor(255, 255, 255));

            // lighting_system->AddLight(spot_light);

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
            VoxelUtil::InitializeSidesUntilMaxDist(10);

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

            voxel_sim.UpdateSimulationDownTop(update_tick);
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
            if (input_manager->IsKeyHeld(Key::R) && cursor_size > 1.2f)
                cursor_size += -0.1f;
            if (input_manager->IsKeyHeld(Key::T) && cursor_size < 10.0f)
                cursor_size += 0.1f;

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
            {
                std::vector<Vector3I> cursor_offsets;

                for (uint8_t i = 0; i < static_cast<uint8_t>(cursor_size); i++)
                {
                    for (const Vector3I& cursor_offset : VoxelUtil::sides_at_dist[i])
                        VoxelUtil::SpawnVoxel(voxel_grid, cursor_center_grid_pos + cursor_offset, selected_voxel);
                }
            }

            if (input_manager->IsKeyHeld(Key::N1))
                selected_voxel = VoxelElement::SAND;

            if (input_manager->IsKeyHeld(Key::N2))
                selected_voxel = VoxelElement::WATER;

            if (input_manager->IsKeyHeld(Key::N3))
                selected_voxel = VoxelElement::STEEL;

            if (input_manager->IsKeyHeld(Key::N4))
                selected_voxel = VoxelElement::STEAM;

            if (input_manager->IsKeyHeld(Key::N5))
                selected_voxel = VoxelElement::LAVA;

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

            // mark cursor positions
            cursor_center_grid_pos = voxel_grid->GetGridPosition(camera->GetPosition() + (camera->GetLookDirection() * cursor_depth));
            bool cursor_was_set    = false;

            std::vector<Vector3I> cursor_offsets;

            for (uint8_t i = 0; i < static_cast<uint8_t>(cursor_size); i++)
            {
                for (const Vector3I& cursor_offset : VoxelUtil::sides_at_dist[i])
                    cursor_offsets.emplace_back(cursor_offset);
            }

            for (const Vector3I& cursor_offset : cursor_offsets)
            {
                Vector3I cursor_pos = cursor_center_grid_pos + cursor_offset;

                if (!voxel_grid->IsPositionInsideGrid(cursor_pos))
                    continue;

                if (!cursor_was_set)
                    prev_cursor_data.clear();

                // save the voxel data that was under this cursor position
                prev_cursor_data.emplace_back(voxel_grid->GetVoxelData(cursor_pos));
                voxel_grid->SetVoxelData(cursor_pos, { VoxelElement::CURSOR, 0 });
                cursor_was_set = true;
            }
            // render voxels
            voxel_scene_renderer.RenderSceneShared(delta);

            // reset the cursor positions to their previous voxel data so the simulation doesn't include the CURSOR type
            if (cursor_was_set)
            {
                for (const Vector3I& cursor_offset : cursor_offsets)
                {
                    Vector3I cursor_pos = cursor_center_grid_pos + cursor_offset;

                    if (!voxel_grid->IsPositionInsideGrid(cursor_pos))
                        continue;

                    voxel_grid->SetVoxelData(cursor_pos, prev_cursor_data.back());
                    prev_cursor_data.pop_back();
                }
            }

            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time);
        }

        std::string DualGame::GetDesiredWindowTitle() const
        {
            return "Selected: " + voxel_element_name_map.at(selected_voxel);
            ;
        }
    }
}