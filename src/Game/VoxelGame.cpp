#include "VoxelGame.hpp"

#include "Math/Util/MathUtil.hpp"

namespace Game
{
    using namespace Display;
    using namespace Engine;
    using namespace Engine::Rendering;
    using namespace Math;

    void VoxelGame::LoadResources()
    {
    }

    VoxelGame::VoxelGame(std::shared_ptr<IFrameDrawer> frame_drawer, std::shared_ptr<IInputManager> input_manager) :
        input_manager(input_manager),
        resource_manager(std::make_shared<ResourceManager>()),
        camera(std::make_shared<Camera>(200, 200, 0.001f, 100.0f, 90.0f)),
        scene_renderer(frame_drawer, resource_manager, camera)
    {
        LoadResources();

        camera->SetPosition(Vector3(0, 0.1f, -1.155f));
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
            shifting  = true;
            mov_speed = 0.001f;
        }
        else if (shifting)
        {
            shifting  = false;
            mov_speed = 0.05f;
        }
    }

    void VoxelGame::Update()
    {
    }

    std::chrono::milliseconds VoxelGame::Render(int64_t delta)
    {
        auto time = std::chrono::high_resolution_clock::now();

        // scene_renderer.DrawShadedMesh(anim_mesh3);

        scene_renderer.RenderScene(delta);

        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time);
    }
}
