#include "RasterGame.hpp"

#include "Math/Util/MathUtil.hpp"

namespace Game
{
    namespace Raster
    {
        using namespace Display;
        using namespace Engine;
        using namespace Engine::Rendering;
        using namespace Math;

        void RasterGame::LoadResources()
        {
            ModelLoadingOptions model_options;
            ModelLoadingOptions model_options_generate_normals;
            model_options_generate_normals.normal_options  = GenerationCondition::ALWAYS;
            model_options_generate_normals.tangent_options = GenerationCondition::ALWAYS;

            resource_manager->LoadModel("../res/bunny.obj", model_options);
            resource_manager->LoadModel("../res/thanks_text.obj", model_options);
            resource_manager->LoadModel("../res/github_text.obj", model_options);
            resource_manager->LoadModel("../res/cube.obj", model_options_generate_normals);
            resource_manager->LoadModel("plane50", model_generator.GeneratePlane(50, 50, 0.0f));
            resource_manager->LoadModel("sphere1", model_generator.GenerateSphere(4));

            resource_manager->LoadModel("../res/alien.md2", model_options);
            resource_manager->LoadModel("../res/marvin.md2", model_options);
            resource_manager->LoadModel("../res/buggy.md2", model_options);
            resource_manager->LoadModel("../res/scarlet.md2", model_options);
            resource_manager->LoadModel("../res/warrior.md2", model_options);
            resource_manager->LoadModel("../res/raptor.md2", model_options_generate_normals);
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

        RasterGame::RasterGame(std::shared_ptr<IInputManager> input_manager) :
            input_manager(std::move(input_manager)),
            resource_manager(std::make_shared<ResourceManager>()),
            lighting_system(std::make_shared<LightingSystem>()),
            camera(std::make_shared<Camera>(200, 200, 0.001f, 100.0f, 90.0f)),
            scene_renderer(resource_manager, lighting_system, camera)
        {
            LoadResources();

            camera->SetPosition(Vector3(0, 0.1f, -4.0f));

            first_floor = StaticMesh();
            first_floor
                .SetModelResource("plane50")
                //.SetTextureResource("../res/tiles.bmp")
                .SetScale(Vector3(12, 12, 12))
                .SetPosition(Vector3(-6, -2, -6));
            marvin = AnimatedMesh();
            marvin
                .SetModelResource("../res/marvin.md2")
                //.SetTextureResource("..res/bricks.bmp")
                .SetPosition(Vector3(0.0f, -0.9f, -1.0f))
                .SetScale(Vector3(0.05f, 0.05f, 0.05f))
                .SetRotation(Angle(-1.7f, 1.7f, 0.0f))
                .SetColor(RGBColor(255, 255, 255));
            buggy = AnimatedMesh();
            buggy
                .SetModelResource("../res/buggy.md2")
                //.SetTextureResource("../res/text.bmp")
                .SetPosition(Vector3(0.0f, -2.0f, 4.0f))
                .SetScale(Vector3(0.1f, 0.1f, 0.1f))
                .SetRotation(Angle(-1.57f, 1.57f * 2, 0.0f))
                .SetColor(RGBColor(255, 255, 255));

            second_floor = StaticMesh();
            second_floor.SetModelResource("plane50")
                .SetTextureResource("../res/tiles.bmp")
                .SetScale(Vector3(12, 12, 12))
                //
                .SetPosition(Vector3(-6, -2, -26));
            wall = StaticMesh();
            wall.SetModelResource("plane50")
                .SetTextureResource("../res/bricks.bmp")
                .SetScale(Vector3(4, 4, 4))
                .SetPosition(Vector3(2, -2, -23))
                //
                .SetRotation(Angle(-1.57f, 1.57f * 2, 0.0f));
            earth = StaticMesh();
            earth.SetModelResource("sphere1")
                .SetTextureResource("../res/earth.bmp")
                .SetPosition(Vector3(0, 0, -20))
                //.SetColor(RGBColor(0, 0, 200))
                .SetRotation(Angle(3.14159f / 2 * 4, 3.14159f * 1.5f, 0))
                .SetScale(Vector3(1.0f, 1.0f, 1.0f));
            //.SetMaterialProperties(MaterialProperties(20.0f, 1.6f));

            third_floor = StaticMesh();
            third_floor.SetModelResource("plane50")
                .SetTextureResource("../res/tiles.bmp")
                .SetScale(Vector3(12, 12, 12))
                //
                .SetPosition(Vector3(-26, -2, -26));
            regular_bunny = StaticMesh();
            regular_bunny.SetModelResource("../res/bunny.obj")
                .SetPosition(Vector3(-20, -2, -24))
                .SetColor(RGBColor(255, 255, 255))
                //
                .SetRotation(Angle(0, 3.14159f / 2 * 4, 0))
                .SetScale(Vector3(10.0f, 10.0f, 10.0f));
            metal_bunny = StaticMesh();
            metal_bunny.SetModelResource("../res/bunny.obj")
                .SetPosition(Vector3(-23, -2, -20))
                .SetColor(RGBColor(255, 0, 00))
                .SetRotation(Angle(0, 3.14159f / 2 * 4, 0))
                .SetScale(Vector3(10.0f, 10.0f, 10.0f))
                .SetMaterialProperties(MaterialProperties(20.0f, 1.6f));
            shiny_bunny = StaticMesh();
            shiny_bunny.SetModelResource("../res/bunny.obj")
                .SetPosition(Vector3(-17, -2, -20))
                .SetColor(RGBColor(0, 255, 0))
                .SetRotation(Angle(0, 3.14159f / 2 * 4, 0))
                .SetScale(Vector3(10.0f, 10.0f, 10.0f))
                .SetMaterialProperties(MaterialProperties(5.0f, 5.4f));
            rough_bunny = StaticMesh();
            rough_bunny.SetModelResource("../res/bunny.obj")
                .SetPosition(Vector3(-20, -2, -16))
                .SetColor(RGBColor(0, 0, 255))
                .SetRotation(Angle(0, 3.14159f / 2 * 4, 0))
                .SetScale(Vector3(10.0f, 10.0f, 10.0f))
                .SetMaterialProperties(MaterialProperties(0.0f, 0.0f));

            fourth_floor = StaticMesh();
            fourth_floor.SetModelResource("plane50")
                .SetTextureResource("../res/tiles.bmp")
                .SetScale(Vector3(12, 12, 12))
                //
                .SetPosition(Vector3(-26, -2, -6));
            regular_wall = StaticMesh();
            regular_wall.SetModelResource("plane50")
                .SetTextureResource("../res/normalmap.bmp")
                .SetScale(Vector3(4, 4, 4))
                .SetPosition(Vector3(-25, -2, -2))
                //
                .SetRotation(Angle(1.57f * 3, 1.57f * 4, 0.0f));
            normal_map_wall = StaticMesh();
            normal_map_wall.SetModelResource("plane50")
                .SetTextureResource("../res/normalmap.bmp")
                .SetNormalMapResource("../res/normalmap.bmp")
                .SetScale(Vector3(4, 4, 4))
                .SetPosition(Vector3(-19, -2, -2))
                //
                .SetRotation(Angle(1.57f * 3, 1.57f * 4, 0.0f));
            brick_wall = StaticMesh();
            brick_wall.SetModelResource("plane50")
                .SetTextureResource("../res/bricks.bmp")
                .SetScale(Vector3(4, 4, 4))
                .SetPosition(Vector3(-25, -2, 2))
                //
                .SetRotation(Angle(1.57f * 3, 1.57f * 4, 0.0f));
            normal_map_brick_wall = StaticMesh();
            normal_map_brick_wall.SetModelResource("plane50")
                .SetTextureResource("../res/bricks.bmp")
                .SetNormalMapResource("../res/bricks_norm.bmp")
                .SetScale(Vector3(4, 4, 4))
                .SetPosition(Vector3(-19, -2, 2))
                //
                .SetRotation(Angle(1.57f * 3, 1.57f * 4, 0.0f));

            thanks_for_watching = StaticMesh();
            thanks_for_watching.SetModelResource("../res/thanks_text.obj")
                .SetPosition(Vector3(-20, 12, -2))
                //.SetColor(RGBColor(0, 0, 255))
                .SetRotation(Angle(3.14159f / 2 * 3, 0.0f, 0))
                .SetScale(Vector3(0.1f, 0.1f, 0.1f));
            github_desc = StaticMesh();
            github_desc.SetModelResource("../res/github_text.obj")
                .SetPosition(Vector3(-20, 12, 0))
                //.SetColor(RGBColor(0, 0, 255))
                .SetRotation(Angle(3.14159f / 2 * 3, 0.0f, 0))
                .SetScale(Vector3(0.1f, 0.1f, 0.1f));

            plight_mesh = StaticMesh();
            plight_mesh.SetModelResource("../res/cube.obj").SetPosition(Vector3(-2.0f, 0.0f, 0.0f));

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
            this->lighting_system->AddLight(spot_light2);
            this->lighting_system->AddLight(spot_light3);

            plight_mesh.SetScale(Vector3(0.1f, 0.1f, 0.1f));
        }

        void RasterGame::SetFrameDrawer(std::shared_ptr<IFrameDrawer> frame_drawer)
        {
            this->frame_drawer = std::move(frame_drawer);

            this->frame_drawer->SetupFrameDrawer();

            Vector3 old_pos     = camera->GetPosition();
            Quaternion old_look = camera->GetRotation();
            camera              = std::make_shared<Camera>(this->frame_drawer->GetFrameBufferWidth(), this->frame_drawer->GetFrameBufferHeight(), 0.001f, 100.0f, 90.0f);
            camera->SetPosition(old_pos);
            camera->SetRotation(old_look);

            scene_renderer = RasterSceneRenderer(resource_manager, lighting_system, camera);
            scene_renderer.SetFrameDrawer(this->frame_drawer);
        };

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
            if (input_manager->IsKeyHeld(Key::LEFT_ARROW))
                camera->RotateYaw(-0.5f);
            if (input_manager->IsKeyHeld(Key::RIGHT_ARROW))
                camera->RotateYaw(0.5f);
            if (input_manager->IsKeyHeld(Key::UP_ARROW))
                camera->RotatePitch(-0.5f);
            if (input_manager->IsKeyHeld(Key::DOWN_ARROW))
                camera->RotatePitch(0.5f);

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

            if (input_manager->IsKeyHeld(Key::MOUSE3))
            {
                dir_light->SetDirection(camera->GetLookDirection());
                point_light->SetPosition(camera->GetPosition());
                plight_mesh.SetPosition(camera->GetPosition());
            }

            if (input_manager->IsKeyHeld(Key::MOUSE2))
            {
                spot_light->SetPosition(camera->GetPosition());
                spot_light->SetDirection(camera->GetLookDirection());
            }
            if (input_manager->IsKeyHeld(Key::MOUSE4))
            {
                spot_light2->SetPosition(camera->GetPosition());
                spot_light2->SetDirection(camera->GetLookDirection());
            }
            if (input_manager->IsKeyHeld(Key::MOUSE5))
            {
                spot_light3->SetPosition(camera->GetPosition());
                spot_light3->SetDirection(camera->GetLookDirection());
            }

            if (input_manager->IsKeyHeld(Key::P))
            {
                marvin.PlayAnimation("taunt", 1.0f);
            }

            if (input_manager->IsKeyHeld(Key::N1))
                second_floor_enabled = true;

            if (input_manager->IsKeyHeld(Key::N2))
                first_floor_enabled = false;

            if (input_manager->IsKeyHeld(Key::N3))
                third_floor_enabled = true;

            if (input_manager->IsKeyHeld(Key::N4))
                second_floor_enabled = false;

            if (input_manager->IsKeyHeld(Key::N5))
                fourth_floor_enabled = true;

            if (input_manager->IsKeyHeld(Key::N6))
            {
                third_floor_enabled = false;
                dir_light->SetColor(RGBColor(0, 0, 0));
            }

            if (input_manager->IsKeyHeld(Key::N7))
            {
                fifth_floor_enabled = true;
                dir_light->SetColor(RGBColor(255, 255, 255));
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

            regular_bunny.SetRotation(Angle(0, i, 0));
            metal_bunny.SetRotation(Angle(0, i, 0));
            shiny_bunny.SetRotation(Angle(0, i, 0));
            rough_bunny.SetRotation(Angle(0, i, 0));

            thanks_for_watching.SetScale(Vector3(std::abs(std::sin(i / 2.0f)) * 0.1f + 0.05f, 0.1f, 0.1f));
            github_desc.SetScale(Vector3(std::abs(std::sin(i / 2.0f)) * 0.1f + 0.05f, 0.1f, 0.1f));
            i += 0.01f;
        }

        std::chrono::milliseconds RasterGame::Render(int64_t delta)
        {
            auto time = std::chrono::high_resolution_clock::now();

            if (first_floor_enabled)
            {
                scene_renderer.DrawShadedMesh(first_floor);
                scene_renderer.DrawShadedMesh(buggy);
                scene_renderer.DrawShadedMesh(marvin);
            }

            if (second_floor_enabled)
            {
                scene_renderer.DrawShadedMesh(second_floor);
                scene_renderer.DrawShadedMesh(earth);
                scene_renderer.DrawShadedMesh(wall);
            }

            if (third_floor_enabled)
            {
                scene_renderer.DrawShadedMesh(third_floor);
                scene_renderer.DrawShadedMesh(regular_bunny);
                scene_renderer.DrawShadedMesh(metal_bunny);
                scene_renderer.DrawShadedMesh(shiny_bunny);
                scene_renderer.DrawShadedMesh(rough_bunny);
            }

            if (fourth_floor_enabled)
            {
                scene_renderer.DrawShadedMesh(fourth_floor);
                scene_renderer.DrawShadedMesh(regular_wall);
                scene_renderer.DrawShadedMesh(normal_map_wall);
                scene_renderer.DrawShadedMesh(brick_wall);
                scene_renderer.DrawShadedMesh(normal_map_brick_wall);
            }

            if (fifth_floor_enabled)
            {
                scene_renderer.DrawShadedMesh(thanks_for_watching);
                scene_renderer.DrawShadedMesh(github_desc);
            }
            // scene_renderer.DrawShadedMesh(mesh);
            //  scene_renderer.DrawShadedMesh(mesh2);

            //	scene_renderer.DrawMesh(plight_mesh);

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

        std::string RasterGame::GetDesiredWindowTitle() const
        {
            return "Raster game";
        }
    }
}
