#ifndef STATICMESH_HPP
#define STATICMESH_HPP

#include "AbstractMesh.hpp"

namespace Engine
{
	namespace Rendering
	{
		class StaticMesh : public AbstractMesh
		{
		public:
			StaticMesh();
			StaticMesh(const Model& model, const Vector3& position);
			StaticMesh(const Model& model, const Vector3& position, const RGBColor& color);
			StaticMesh(const Model& model, const Vector3& position, const Angle& rotation);
			StaticMesh(const Model& model, const Vector3& position, const Angle& rotation, const RGBColor& color);

			virtual void DrawMesh(Rasterizer& rasterizer) const;
		};
	}
}

#endif