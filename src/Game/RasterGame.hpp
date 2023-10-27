#ifndef RASTERGAME_HPP
#define RASTERGAME_HPP

#include "IGame.hpp"

#include "Display/IFrameDrawer.hpp"
#include "Engine/Input/IInputManager.hpp"
#include "Engine/Rendering/AnimatedMesh.hpp"
#include "Engine/Rendering/Camera.hpp"
#include "Engine/Rendering/Lighting/DirectionalLight.hpp"
#include "Engine/Rendering/Lighting/ILight.hpp"
#include "Engine/Rendering/Lighting/LightingSystem.hpp"
#include "Engine/Rendering/Lighting/PointLight.hpp"
#include "Engine/Rendering/Lighting/SpotLight.hpp"
#include "Engine/Rendering/RasterSceneRenderer.hpp"
#include "Engine/Rendering/Rasterizer.hpp"
#include "Engine/Rendering/StaticMesh.hpp"
#include "Engine/Rendering/Transform.hpp"
#include "Engine/Rendering/Vertex.hpp"
#include "Engine/Resources/ModelGenerator.hpp"
#include "Engine/Resources/ResourceManager.hpp"
#include "Math/Vector3.hpp"

#include <chrono>
#include <cstdint>
#include <memory>

namespace Game
{
    using namespace Display;
    using namespace Engine;
    using namespace Engine::Rendering;
    using namespace Math;
    using namespace Lighting;
    using namespace Resources;
    using namespace Input;

    class RasterGame : public IGame
    {
    private:
        std::shared_ptr<IInputManager> input_manager;

        ModelGenerator model_generator;
        std::shared_ptr<ResourceManager> resource_manager;
        std::shared_ptr<LightingSystem> lighting_system;
        std::shared_ptr<Camera> camera;

        RasterSceneRenderer scene_renderer;

        StaticMesh mesh;
        StaticMesh mesh2;
        StaticMesh mesh3;

        AnimatedMesh anim_mesh;
        AnimatedMesh anim_mesh2;
        AnimatedMesh anim_mesh3;
        StaticMesh floor;

        std::shared_ptr<DirectionalLight> dir_light;
        std::shared_ptr<PointLight> point_light;
        std::shared_ptr<SpotLight> spot_light;
        std::shared_ptr<SpotLight> spot_light2;
        std::shared_ptr<SpotLight> spot_light3;
        StaticMesh plight_mesh;

        virtual void LoadResources() override;

        float rot       = 1.33f;
        float mov_speed = 0.05f;
        bool shifting   = false;

    public:
        RasterGame(std::shared_ptr<IFrameDrawer> frame_drawer, std::shared_ptr<IInputManager> input_manager);

        virtual void HandleInput() override;
        virtual void Update() override;
        virtual std::chrono::milliseconds Render(int64_t delta) override;
    };
}

#endif
