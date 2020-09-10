#include "Vertex.hpp"

namespace Engine
{
	namespace Rendering
	{
		Vector3 Vertex::GetPosition() const
		{
			return position;
		}

		Vertex& Vertex::SetPosition(const Vector3& position)
		{
			this->position = position;

			return *this;
		}

		Vector3 Vertex::GetNormal() const
		{
			return normal;
		}

		Vertex& Vertex::SetNormal(const Vector3& normal)
		{
			this->normal = normal;

			return *this;
		}

		float Vertex::GetW() const
		{
			return w;
		}

		Vertex& Vertex::SetW(float w)
		{
			this->w = w;

			return *this;
		}

		Vertex Vertex::GetPerspectiveDivided() const
		{
			return Vertex(*this).PerspectiveDivide();
		}

		Vertex& Vertex::PerspectiveDivide()
		{
			position.x /= w;
			position.y /= w;
			position.z /= w;

			return *this;
		}

	}
}
