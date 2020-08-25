#include "Vertex.hpp"

namespace Engine
{
	namespace Rendering
	{
		const Vector3& Vertex::GetPosition() const
		{
			return position;
		}

		Vertex& Vertex::SetPosition(const Vector3& position)
		{
			this->position = position;
		}

		const Vector3& Vertex::GetNormal() const
		{
			return normal;
		}

		Vertex& Vertex::SetNormal(const Vector3& normal)
		{
			this->normal = normal;
		}

		Vertex& Vertex::PerspectiveDivide()
		{
			//TODO: move homogenous coordinate to this class
			position.PerspectiveDivide();
		}

	}
}
