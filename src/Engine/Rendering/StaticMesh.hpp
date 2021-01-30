#ifndef STATICMESH_HPP
#define STATICMESH_HPP

#include "AbstractMesh.hpp"

#include <memory>

namespace Engine
{
	namespace Rendering
	{
		class StaticMesh : public AbstractMesh
		{
		public:
			using AbstractMesh::AbstractMesh;

			virtual bool IsAnimated() const override;
		};
	}
}

#endif
