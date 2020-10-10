#include "Vertex.hpp"

#include "../../Math/Util/MathUtil.hpp"

#include <cmath>

using namespace Math;

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

		bool Vertex::IsInsideViewFrustum() const
		{
			return std::fabs(position.x) < std::fabs(w) && std::fabs(position.y) < std::fabs(w) && std::fabs(position.z) < std::fabs(w);
		}

		Vertex& Vertex::Lerp(const Vertex& other, float amount)
		{
			position.Lerp(other.GetPosition(), amount);
			normal.Lerp(other.GetNormal(), amount);
			w = Util::Lerp(amount, w, other.GetW());

			return *this;
		}

		[[nodiscard]] Vertex Vertex::GetLerped(const Vertex& other, float amount) const
		{
			return Vertex(*this).Lerp(other, amount);
		}
	}
}
