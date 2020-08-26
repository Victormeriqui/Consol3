#include "StaticMesh.hpp"

namespace Engine
{
	namespace Rendering
	{
		StaticMesh::StaticMesh(const Model& model, const Vector3& position) : AbstractMesh(model, position)
		{
		}

		StaticMesh::StaticMesh(const Model& model, const Vector3& position, const Color& color) : AbstractMesh(model, position, color)
		{
		}

		StaticMesh::StaticMesh(const Model& model, const Vector3& position, const Angle& rotation) : AbstractMesh(model, position, rotation)
		{
		}

		StaticMesh::StaticMesh(const Model& model, const Vector3& position, const Angle& rotation, const Color& color) : AbstractMesh(model, position, rotation, color)
		{
		}

		void StaticMesh::DrawMesh() const
		{
			//TODO : implement this
		}

	}
}
