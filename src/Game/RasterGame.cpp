#include "RasterGame.hpp"

#include "Math/Util/MathUtil.hpp"

namespace Game
{
    using namespace Display;
    using namespace Engine;
    using namespace Engine::Rendering;
    using namespace Math;

    void RasterGame::LoadResources()
    {
        ModelLoadingOptions model_options;
        resource_manager->LoadModel("../res/bunny.obj", model_options);
        resource_manager->LoadModel("../res/cube.obj", model_options);
        resource_manager->LoadModel("plane50", model_generator.GeneratePlane(50, 50, 0.0f));
        resource_manager->LoadModel("sphere1", model_generator.GenerateSphere(4));

        resource_manager->LoadModel("../res/alien.md2", model_options);
        resource_manager->LoadModel("../res/marvin.md2", model_options);
        resource_manager->LoadModel("../res/buggy.md2", model_options);
        resource_manager->LoadModel("../res/scarlet.md2", model_options);
        resource_manager->LoadModel("../res/warrior.md2", model_options);
        resource_manager->LoadModel("../res/raptor.md2", model_options);
        resource_manager->LoadModel("../res/penguin.md2", model_options);
        resource_manager->LoadModel("../res/centaur.md2", model_options);

        resource_manager->LoadTexture("../res/tiles.bmp", TextureLoadingOptions::DEFAULT);
        resource_manager->LoadTexture("../res/tnt.bmp", TextureLoadingOptions::DEFAULT);
        resource_manager->LoadTexture("../res/text.bmp", TextureLoadingOptions::DEFAULT);
        resource_manager->LoadTexture("../res/bricks.bmp", TextureLoadingOptions::FLIP_Y);
        resource_manager->LoadTexture("../res/bricks_norm.bmp", TextureLoadingOptions::DEFAULT);
        resource_manager->LoadTexture("../res/raptor.bmp", TextureLoadingOptions::FLIP_Y);
        resource_manager->LoadTexture("../res/penguin.bmp", TextureLoadingOptions::FLIP_Y);
        resource_manager->LoadTexture("../res/centaur.bmp", TextureLoadingOptions::FLIP_Y);
        resource_manager->LoadTexture("../res/earth.bmp", TextureLoadingOptions::DEFAULT);
        resource_manager->LoadTexture("../res/normalmap.bmp", TextureLoadingOptions::DEFAULT);
    }

    RasterGame::RasterGame(std::shared_ptr<IFrameDrawer> frame_drawer, std::shared_ptr<IInputManager> input_manager) :
        input_manager(std::move(input_manager)),
        resource_manager(std::make_shared<ResourceManager>()),
        lighting_system(std::make_shared<LightingSystem>()),
        camera(std::make_shared<Camera>(frame_drawer->GetFrameBufferWidth(), frame_drawer->GetFrameBufferHeight(), 0.001f, 100.0f, 90.0f)),
        scene_renderer(frame_drawer, resource_manager, lighting_system, camera)
    {
        LoadResources();

        camera->SetPosition(Vector3(0, 0.1f, -1.155f));

        anim_mesh = AnimatedMesh();
        anim_mesh
            .SetModelResource("../res/marvin.md2")
            //.SetTextureResource("res/text.bmp")
            .SetPosition(Vector3(1.0f, -0.9f, 0.0f))
            .SetScale(Vector3(0.05f, 0.05f, 0.05f))
            .SetRotation(Angle(-1.7f, 0.0f, 0.0f))
            .SetColor(RGBColor(255, 255, 255));

        anim_mesh2 = AnimatedMesh();
        anim_mesh2
            .SetModelResource("../res/penguin.md2")
            //.SetTextureResource("res/text.bmp")
            .SetPosition(Vector3(-2.0f, -0.9f, 0.0f))
            .SetScale(Vector3(0.05f, 0.05f, 0.05f))
            .SetRotation(Angle(-1.7f, 0.0f, 0.0f))
            .SetColor(RGBColor(255, 255, 255));

        anim_mesh3 = AnimatedMesh();
        anim_mesh3
            .SetModelResource("../res/marvin.md2")
            //.SetTextureResource("res/text.bmp")
            .SetPosition(Vector3(0.0f, -0.9f, 1.0f))
            .SetScale(Vector3(0.05f, 0.05f, 0.05f))
            .SetRotation(Angle(-1.7f, 0.0f, 0.0f))
            .SetColor(RGBColor(0, 0, 255));

        mesh = StaticMesh();
        mesh.SetModelResource("../res/bunny.obj")
            .SetPosition(Vector3(2, 1, 0))
            //.SetColor(RGBColor(0, 0, 200))
            .SetRotation(Angle(0, 3.14159f / 2 * 4, 0))
            .SetScale(Vector3(10.0f, 10.0f, 10.0f))
            .SetMaterialProperties(MaterialProperties(20.0f, 1.6f));

        mesh2 = StaticMesh();
        mesh2.SetModelResource("../res/bunny.obj")
            .SetPosition(Vector3(-2, 1, 0))
            //.SetColor(RGBColor(0, 0, 255))
            .SetRotation(Angle(0, 3.14159f / 2 * 4, 0))
            .SetScale(Vector3(10.0f, 10.0f, 10.0f))
            .SetMaterialProperties(MaterialProperties(0.0f, 0.0f));

        plight_mesh = StaticMesh();
        plight_mesh.SetModelResource("../res/cube.obj").SetPosition(Vector3(-2.0f, 0.0f, 0.0f));

        floor = StaticMesh();
        floor.SetModelResource("plane50").SetTextureResource("../res/tiles.bmp").SetScale(Vector3(12, 12, 12)).SetPosition(Vector3(-6, -2, -6));

        dir_light = std::make_shared<DirectionalLight>(Vector3(-1, -0.5f, 0));
        dir_light->SetColor(RGBColor(255, 255, 255));

        point_light = std::make_shared<PointLight>(Vector3(-2, 0, 0));
        point_light->SetRange(25.0f);
        point_light->SetColor(RGBColor(255, 0, 0));

        spot_light = std::make_shared<SpotLight>(Vector3(0, 0.1f, -3.0f), Vector3(0, 0, 1));
        spot_light->SetRange(25.0f);
        spot_light->SetAngle(20.0f);
        spot_light->SetColor(RGBColor(255, 0, 0));

        spot_light2 = std::make_shared<SpotLight>(Vector3(0, 0.1f, -3.0f), Vector3(0, 0, 1));
        spot_light2->SetRange(15.0f);
        spot_light2->SetAngle(20.0f);
        spot_light2->SetColor(RGBColor(0, 255, 0));

        spot_light3 = std::make_shared<SpotLight>(Vector3(0, 0.1f, -3.0f), Vector3(0, 0, 1));
        spot_light3->SetRange(15.0f);
        spot_light3->SetAngle(20.0f);
        spot_light3->SetColor(RGBColor(0, 0, 255));

        this->lighting_system->SetAmbientLightColor(RGBColor(10, 10, 10));
        this->lighting_system->AddLight(dir_light);
        // this->lighting_system->AddLight(point_light);
        this->lighting_system->AddLight(spot_light);
        // this->lighting_system->AddLight(spot_light2);
        // this->lighting_system->AddLight(spot_light3);

        plight_mesh.SetScale(Vector3(0.1f, 0.1f, 0.1f));
    }

    void RasterGame::HandleInput()
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

        if (input_manager->IsKeyHeld(Key::MOUSE1))
        {
            dir_light->SetDirection(camera->GetLookDirection());
            point_light->SetPosition(camera->GetPosition());
            plight_mesh.SetPosition(camera->GetPosition());

            spot_light2->SetPosition(camera->GetPosition());
            spot_light2->SetDirection(camera->GetLookDirection());
        }

        if (input_manager->IsKeyHeld(Key::MOUSE2))
        {
            spot_light->SetPosition(camera->GetPosition());
            spot_light->SetDirection(camera->GetLookDirection());
        }
        if (input_manager->IsKeyHeld(Key::MOUSE3))
        {
            spot_light3->SetPosition(camera->GetPosition());
            spot_light3->SetDirection(camera->GetLookDirection());
        }
    }

    float i = 0;
    void RasterGame::Update()
    {
        // point_light->SetPosition(Vector3(std::sin(i) * 2, 0.5, std::cos(i) * 2));
        // plight_mesh.SetPosition(Vector3(std::sin(i) * 2, 0.5, std::cos(i) * 2));
        // anim_mesh.SetPosition(Vector3(std::sin(i) * 2, -0.9f, std::cos(i) * 2));
        // anim_mesh2.SetPosition(Vector3(std::sin(i + 2) * 2, -0.9f, std::cos(i + 2) * 2));
        // anim_mesh3.SetPosition(Vector3(std::sin(i + 4) * 2, -0.9f, std::cos(i + 4) * 2));

        i += 0.01f;
    }

    std::chrono::milliseconds RasterGame::Render(int64_t delta)
    {
        auto time = std::chrono::high_resolution_clock::now();

        scene_renderer.DrawShadedMesh(floor);

        // scene_renderer.DrawShadedMesh(mesh);
        // scene_renderer.DrawShadedMesh(mesh2);

        //	scene_renderer.DrawMesh(plight_mesh);

        scene_renderer.DrawShadedMesh(anim_mesh);
        scene_renderer.DrawShadedMesh(anim_mesh2);
        // scene_renderer.DrawShadedMesh(anim_mesh3);

        scene_renderer.RenderScene(delta);
        /*
        for (int y = 0; y < 200; y++)
        {
                for (int x = 0; x < 200; x++)
                {
                        float z		= dir_light->GetLightDepthBuffer().value().get().GetValue(x, y);
                        uint16_t nx = Util::LerpCast<uint16_t>(x / 200.0f, 0, 50);
                        uint16_t ny = Util::LerpCast<uint16_t>(y / 200.0f, 0, 50);
                        scene_renderer.DrawPixel(nx, ny, HSVColor(0, 0, Util::Lerp(z, 0, 1)));
                }
        }

        for (int y = 0; y < 200; y++)
        {
                for (int x = 0; x < 200; x++)
                {
                        float z		= spot_light->GetLightDepthBuffer().value().get().GetValue(x, y);
                        uint16_t nx = Util::LerpCast<uint16_t>(x / 200.0f, 0, 50);
                        uint16_t ny = Util::LerpCast<uint16_t>(y / 200.0f, 0, 50);
                        scene_renderer.DrawPixel(50 + nx, ny, HSVColor(0, 0, Util::Lerp(z, 0, 1)));
                }
        }


        for (int y = 0; y < 200; y++)
        {
                for (int x = 0; x < 200; x++)
                {
                        float z = spot_light2->GetLightDepthBuffer().value().get().GetValue(x, y);
                        uint16_t nx = Util::LerpCast<uint16_t>(x / 200.0f, 0, 50);
                        uint16_t ny = Util::LerpCast<uint16_t>(y / 200.0f, 0, 50);
                        scene_renderer.DrawPixel(150+nx, ny, HSVColor(0, 0, Util::Lerp(z, 0, 1)));
                }
        }*/

        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time);
    }
}
