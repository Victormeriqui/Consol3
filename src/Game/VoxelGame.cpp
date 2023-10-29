#include "VoxelGame.hpp"

#include "Display/RGBColor.hpp"
#include "Math/Util/MathUtil.hpp"
#include "Math/Vector3.hpp"

using namespace Math;
using namespace Math::Util;

#include <algorithm>
#include <random>

namespace Game
{
    using namespace Display;
    using namespace Engine;
    using namespace Engine::Rendering;
    using namespace Math;

    VoxelGame::VoxelGame(std::shared_ptr<IFrameDrawer> frame_drawer, std::shared_ptr<IInputManager> input_manager) :
        input_manager(input_manager),
        resource_manager(std::make_shared<ResourceManager>()),
        camera(std::make_shared<Camera>(frame_drawer->GetFrameBufferWidth(), frame_drawer->GetFrameBufferHeight(), 0.001f, 100.0f, 90.0f)),
        voxel_grid(std::make_shared<VoxelGrid<VOXEL_GRID_WIDTH, VOXEL_GRID_HEIGHT, VOXEL_GRID_DEPTH>>(0)),
        scene_renderer(frame_drawer, resource_manager, camera, voxel_grid),
        random_generator(std::random_device()())
    {
        LoadResources();

        camera->SetPosition(Vector3(40, 40, 30));

        std::map<uint8_t, RGBColor> voxel_palette = ConvertTypeMapToPalette(voxel_type_map);
        scene_renderer.SetVoxelPalette(std::make_unique<std::map<uint8_t, RGBColor>>(voxel_palette));
    }

    VoxelType VoxelGame::GetVoxel(const Vector3& pos) const
    {
        return static_cast<VoxelType>(voxel_grid->GetVoxel(static_cast<uint16_t>(pos.x), static_cast<uint16_t>(pos.y), static_cast<uint16_t>(pos.z)));
    }
    VoxelType VoxelGame::GetVoxel(uint16_t x, uint16_t y, uint16_t z) const
    {
        return static_cast<VoxelType>(voxel_grid->GetVoxel(x, y, z));
    }

    void VoxelGame::SetVoxel(const Vector3& pos, VoxelType voxel)
    {
        voxel_grid->SetVoxel(static_cast<uint16_t>(pos.x), static_cast<uint16_t>(pos.y), static_cast<uint16_t>(pos.z), static_cast<uint8_t>(voxel));
    }
    void VoxelGame::SetVoxel(uint16_t x, uint16_t y, uint16_t z, VoxelType voxel)
    {
        voxel_grid->SetVoxel(x, y, z, static_cast<uint8_t>(voxel));
    }

    void VoxelGame::LoadResources()
    {
        for (uint16_t z = 0; z < VOXEL_GRID_DEPTH; z++)
        {
            for (uint16_t x = 0; x < VOXEL_GRID_WIDTH; x++)
            {
                SetVoxel(x, 0, z, VoxelType::ROCK);
            }
        }
    }

    static std::array<Vector3, 4> side_down_options = { Vector3(-1, -1, 0), Vector3(1, -1, 0), Vector3(0, -1, -1), Vector3(0, -1, 1) };
    static std::array<Vector3, 4> side_options      = { Vector3(-1, 0, 0), Vector3(1, 0, 0), Vector3(0, 0, -1), Vector3(0, 0, 1) };
    void VoxelGame::Update()
    {
        for (uint16_t y = 0; y < VOXEL_GRID_HEIGHT; y++)
        {
            for (uint16_t x = 0; x < VOXEL_GRID_WIDTH; x++)
            {
                for (uint16_t z = 0; z < VOXEL_GRID_DEPTH; z++)
                {
                    VoxelType cur_voxel = GetVoxel(x, y, z);

                    if (cur_voxel == VoxelType::SAND || cur_voxel == VoxelType::WATER)
                    {
                        VoxelType down_voxel = GetVoxel(x, y - 1, z);

                        if (down_voxel == VoxelType::AIR)
                        {
                            SetVoxel(x, y, z, VoxelType::AIR);
                            SetVoxel(x, y - 1, z, cur_voxel);
                        }
                        else
                        {
                            bool moved_down_side                          = false;
                            std::array<Vector3, 4> rand_side_down_options = side_down_options;
                            // randomize the order so theres no spreading pattern
                            std::shuffle(rand_side_down_options.begin(), rand_side_down_options.end(), random_generator);

                            for (const Vector3& side_option : rand_side_down_options)
                            {
                                Vector3 side_pos     = Vector3(static_cast<float>(x + side_option.x), static_cast<float>(y + side_option.y), static_cast<float>(z + side_option.z));
                                VoxelType side_voxel = GetVoxel(side_pos);

                                if (side_voxel == VoxelType::AIR && IsInRange(side_pos.x, 5, VOXEL_GRID_WIDTH - 5) && IsInRange(side_pos.y, 0, VOXEL_GRID_HEIGHT) && IsInRange(side_pos.z, 5, VOXEL_GRID_DEPTH - 5))
                                {
                                    SetVoxel(x, y, z, VoxelType::AIR);
                                    SetVoxel(side_pos, cur_voxel);
                                    moved_down_side = true;
                                    break;
                                }
                            }

                            if (cur_voxel == VoxelType::WATER && !moved_down_side)
                            {
                                std::array<Vector3, 4> rand_side_options = side_options;

                                std::shuffle(rand_side_options.begin(), rand_side_options.end(), random_generator);

                                for (const Vector3& side_option : rand_side_options)
                                {
                                    Vector3 side_pos     = Vector3(static_cast<float>(x + side_option.x), static_cast<float>(y + side_option.y), static_cast<float>(z + side_option.z));
                                    VoxelType side_voxel = GetVoxel(side_pos);

                                    if (side_voxel == VoxelType::AIR && IsInRange(side_pos.x, 5, VOXEL_GRID_WIDTH - 5) && IsInRange(side_pos.y, 0, VOXEL_GRID_HEIGHT) && IsInRange(side_pos.z, 5, VOXEL_GRID_DEPTH - 5))
                                    {
                                        SetVoxel(x, y, z, VoxelType::AIR);
                                        SetVoxel(side_pos, cur_voxel);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        increasing_counter += 0.005f;
    }

    void VoxelGame::HandleInput()
    {
        if (input_manager->IsKeyPressed(Key::CAPITAL))
        {
            Vector2 mouse_dist_center = input_manager->GetMouseDistanceToCenter();

            if (mouse_dist_center.x != 0)
                camera->RotateYaw(mouse_dist_center.x * 0.1f);
            if (mouse_dist_center.y != 0)
                camera->RotatePitch(mouse_dist_center.y * 0.1f);

            input_manager->SetMousePositionToCenter();
        }

        if (input_manager->IsKeyHeld(Key::MOUSE2))
        {
            Vector3 camera_pos = camera->GetPosition();
            SetVoxel(static_cast<uint16_t>(camera_pos.x), static_cast<uint16_t>(camera_pos.y), static_cast<uint16_t>(camera_pos.y), VoxelType::SAND);
        }

        if (input_manager->IsKeyHeld(Key::MOUSE4))
        {
            Vector3 camera_pos = camera->GetPosition();
            SetVoxel(static_cast<uint16_t>(camera_pos.x), static_cast<uint16_t>(camera_pos.y), static_cast<uint16_t>(camera_pos.y), VoxelType::WATER);
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
            mov_speed = 0.1f;
        }
        else if (shifting)
        {
            shifting  = false;
            mov_speed = 0.05f;
        }
    }

    std::chrono::milliseconds VoxelGame::Render(int64_t delta)
    {
        auto time = std::chrono::high_resolution_clock::now();

        // scene_renderer.DrawShadedMesh(anim_mesh3);

        scene_renderer.RenderScene(delta);

        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time);
    }
}
