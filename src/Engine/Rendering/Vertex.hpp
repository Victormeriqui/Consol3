#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "../../Math/Vector3.hpp"
#include "../../Math/Matrix4.hpp"

namespace Engine
{
	using namespace Math;

	namespace Rendering
	{
		class Vertex
		{
		private:
			Vector3 position;
			// w for homogenous coordinates for matrix multiplication and perspective divide
			float w;

			Vector3 normal;

		public:
			constexpr Vertex() : position(Vector3()), w(1), normal(Vector3()) {}
			constexpr Vertex(Vector3 position) : position(position), w(1), normal(Vector3(0, 0, 0)) {}
			constexpr Vertex(Vector3 position, Vector3 normal) : position(position), w(1), normal(normal) {};

			[[nodiscard]] Vector3 GetPosition() const;
			Vertex& SetPosition(const Vector3& position);

			[[nodiscard]] Vector3 GetNormal() const;
			Vertex& SetNormal(const Vector3& normal);

			[[nodiscard]] float GetW() const;
			Vertex& SetW(float w);

			[[nodiscard]] Vertex GetPerspectiveDivided() const;
			Vertex& PerspectiveDivide();

			[[nodiscard]] bool IsInsideViewFrustum() const;

			[[nodiscard]] Vertex GetLerped(const Vertex& other, float amount) const;
			Vertex& Lerp(const Vertex& other, float amount);

			Vertex& TransformNormals(const Matrix4& normal_mat);

			constexpr Vertex& operator*= (const Matrix4& mat) noexcept
			{
				float x_new = mat.values[0][0] * position.x + mat.values[0][1] * position.y + mat.values[0][2] * position.z + mat.values[0][3] * w;
				float y_new = mat.values[1][0] * position.x + mat.values[1][1] * position.y + mat.values[1][2] * position.z + mat.values[1][3] * w;
				float z_new = mat.values[2][0] * position.x + mat.values[2][1] * position.y + mat.values[2][2] * position.z + mat.values[2][3] * w;
				float w_new = mat.values[3][0] * position.x + mat.values[3][1] * position.y + mat.values[3][2] * position.z + mat.values[3][3] * w;

				position.x = x_new;
				position.y = y_new;
				position.z = z_new;
				w = w_new;

				return *this;
			}

			[[nodiscard]] constexpr Vertex operator* (const Matrix4& mat) const noexcept
			{
				return Vertex(*this) *= mat;
			}
		};
	}
}

#endif