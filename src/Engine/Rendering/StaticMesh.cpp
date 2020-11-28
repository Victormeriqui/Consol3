#include "StaticMesh.hpp"

#include "Camera.hpp"

namespace Engine
{
	namespace Rendering
	{


		void StaticMesh::DrawMesh(Camera& camera, Rasterizer& rasterizer) const
		{
			if (has_texture)
				model.DrawModel(transform, camera.GetDepthBuffer(), rasterizer, std::make_shared<Texture>(texture), hsvcolor);
			else
				model.DrawModel(transform, camera.GetDepthBuffer(), rasterizer, hsvcolor);
		}
	}
}