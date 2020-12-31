#include "StaticMesh.hpp"

#include "Camera.hpp"

#include <memory>

namespace Engine
{
	namespace Rendering
	{
		void StaticMesh::DrawMesh(Camera& camera, std::shared_ptr<LightingSystem> lighting_system, Rasterizer& rasterizer) const
		{
			lighting_system->RenderToDepthBuffers(model, transform);

			if (has_texture)
				model.DrawTexturedModel(transform, camera.GetDepthBuffer(), rasterizer, std::make_shared<Texture>(texture), hsvcolor);
			else
				model.DrawModel(transform, camera.GetDepthBuffer(), rasterizer, hsvcolor);
		}

		void StaticMesh::DrawShadedMesh(Camera& camera, std::shared_ptr<LightingSystem> lighting_system, Rasterizer& rasterizer) const
		{
			lighting_system->RenderToDepthBuffers(model, transform);

			if (has_texture)
				model.DrawTexturedAndShadedModel(transform, camera.GetDepthBuffer(), rasterizer, lighting_system, std::make_shared<Texture>(texture), hsvcolor);
			else
				model.DrawShadedModel(transform, camera.GetDepthBuffer(), rasterizer, lighting_system, hsvcolor);
		}
	}
}