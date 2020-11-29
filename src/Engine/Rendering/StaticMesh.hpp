#ifndef STATICMESH_HPP
#define STATICMESH_HPP

#include "AbstractMesh.hpp"
#include "Camera.hpp"

#include <memory>

namespace Engine
{
	namespace Rendering
	{
		class StaticMesh : public AbstractMesh
		{
		public:
			using AbstractMesh::AbstractMesh;

			virtual void DrawMesh(Camera& camera, Rasterizer& rasterizer) const override;
			virtual void DrawShadedMesh(Camera& camera, std::shared_ptr<LightingSystem> lighting_system, Rasterizer& rasterizer) const override;
		};
	}
}

#endif