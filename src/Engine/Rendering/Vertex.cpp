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

		Vector3 Vertex::GetTangent() const
		{
			return tangent;
		}

		Vertex& Vertex::SetTangent(const Vector3& tangent)
		{
			this->tangent = tangent;

			return *this;
		}

		Vector3 Vertex::GetBitangent() const
		{
			return bitangent;
		}

		Vertex& Vertex::SetBitangent(const Vector3& bitangent)
		{
			this->bitangent = bitangent;

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

		Vertex& Vertex::TransformNormal(const Matrix4& normal_mat)
		{
			float norm_x_new = normal_mat.values[0][0] * normal.x + normal_mat.values[0][1] * normal.y + normal_mat.values[0][2] * normal.z;
			float norm_y_new = normal_mat.values[1][0] * normal.x + normal_mat.values[1][1] * normal.y + normal_mat.values[1][2] * normal.z;
			float norm_z_new = normal_mat.values[2][0] * normal.x + normal_mat.values[2][1] * normal.y + normal_mat.values[2][2] * normal.z;
			//float norm_w_new = normal_mat.values[3][0] * normal.x + normal_mat.values[3][1] * normal.y + normal_mat.values[3][2] * normal.z;

			normal.x = norm_x_new;
			normal.y = norm_y_new;
			normal.z = norm_z_new;

			normal.Normalize();

			return *this;
		}

		Vertex& Vertex::TransformTangent(const Matrix4& normal_mat)
		{
			float tang_x_new = normal_mat.values[0][0] * tangent.x + normal_mat.values[0][1] * tangent.y + normal_mat.values[0][2] * tangent.z;
			float tang_y_new = normal_mat.values[1][0] * tangent.x + normal_mat.values[1][1] * tangent.y + normal_mat.values[1][2] * tangent.z;
			float tang_z_new = normal_mat.values[2][0] * tangent.x + normal_mat.values[2][1] * tangent.y + normal_mat.values[2][2] * tangent.z;
			//float tang_w_new = normal_mat.values[3][0] * tangent.x + normal_mat.values[3][1] * tangent.y + normal_mat.values[3][2] * tangent.z;

			tangent.x = tang_x_new;
			tangent.y = tang_y_new;
			tangent.z = tang_z_new;

			tangent.Normalize();

			return *this;
		}

		Vertex& Vertex::TransformBitangent(const Matrix4& normal_mat)
		{
			float bitang_x_new =
				normal_mat.values[0][0] * bitangent.x + normal_mat.values[0][1] * bitangent.y + normal_mat.values[0][2] * bitangent.z;
			float bitang_y_new =
				normal_mat.values[1][0] * bitangent.x + normal_mat.values[1][1] * bitangent.y + normal_mat.values[1][2] * bitangent.z;
			float bitang_z_new =
				normal_mat.values[2][0] * bitangent.x + normal_mat.values[2][1] * bitangent.y + normal_mat.values[2][2] * bitangent.z;
			//float bitang_w_new =
			//	normal_mat.values[3][0] * bitangent.x + normal_mat.values[3][1] * bitangent.y + normal_mat.values[3][2] * bitangent.z;

			bitangent.x = bitang_x_new;
			bitangent.y = bitang_y_new;
			bitangent.z = bitang_z_new;

			bitangent.Normalize();

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
