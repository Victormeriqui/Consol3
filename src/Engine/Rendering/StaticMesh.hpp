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
		};
	}
}

#endif
