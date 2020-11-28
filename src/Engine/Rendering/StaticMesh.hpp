#ifndef STATICMESH_HPP
#define STATICMESH_HPP

#include "AbstractMesh.hpp"
#include "Camera.hpp"

namespace Engine
{
	namespace Rendering
	{
		class StaticMesh : public AbstractMesh
		{
		public:
			using AbstractMesh::AbstractMesh;

			virtual void DrawMesh(Camera& camera, Rasterizer& rasterizer) const;
		};
	}
}

#endif