#include "Vertex.hpp"

#include "../../Math/Util/MathUtil.hpp"

#include <cmath>

namespace Engine
{
	namespace Rendering
	{
		using namespace Math;

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

		Vector2 Vertex::GetTextureCoords() const
		{
			return texture_coords;
		}

		Vertex& Vertex::SetTextureCoords(const Vector2& texture_coords)
		{
			this->texture_coords = texture_coords;

			return *this;
		}

		Vertex& Vertex::TransformNormals(const Matrix4& normal_mat)
		{
			float norm_x_new = normal_mat.values[0][0] * normal.x + normal_mat.values[0][1] * normal.y + normal_mat.values[0][2] * normal.z;
			float norm_y_new = normal_mat.values[1][0] * normal.x + normal_mat.values[1][1] * normal.y + normal_mat.values[1][2] * normal.z;
			float norm_z_new = normal_mat.values[2][0] * normal.x + normal_mat.values[2][1] * normal.y + normal_mat.values[2][2] * normal.z;
			float norm_w_new = normal_mat.values[3][0] * normal.x + normal_mat.values[3][1] * normal.y + normal_mat.values[3][2] * normal.z;

			normal.x = norm_x_new;
			normal.y = norm_y_new;
			normal.z = norm_z_new;

			normal.Normalize();

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
			texture_coords.Lerp(other.GetTextureCoords(), amount);

			w = Util::Lerp(amount, w, other.GetW());

			return *this;
		}

		[[nodiscard]] Vertex Vertex::GetLerped(const Vertex& other, float amount) const
		{
			return Vertex(*this).Lerp(other, amount);
		}
	}
}
