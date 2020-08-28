#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "Vector3.hpp"
#include "Matrix4.hpp"

using namespace Engine::Math;

namespace Engine
{
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
			constexpr Vertex(Vector3 position) : position(position), w(0), normal(Vector3(0, 0, 0)) {}
			constexpr Vertex(Vector3 position, Vector3 normal) : position(position), w(0), normal(normal) {};

			[[nodiscard]] Vector3 GetPosition() const;
			Vertex& SetPosition(const Vector3& position);

			[[nodiscard]] Vector3 GetNormal() const;
			Vertex& SetNormal(const Vector3& normal);

			[[nodiscard]] float GetW() const;
			Vertex& SetW(float w);

			[[nodiscard]] Vertex GetPerspectiveDivided() const;
			Vertex& PerspectiveDivide();

			constexpr Vertex& operator*= (const Matrix4& mat) noexcept
			{
				position.x = mat.values[0][0] * position.x + mat.values[0][1] * position.y + mat.values[0][2] * position.z + mat.values[0][3] * w;
				position.y = mat.values[1][0] * position.x + mat.values[1][1] * position.y + mat.values[1][2] * position.z + mat.values[1][3] * w;
				position.z = mat.values[2][0] * position.x + mat.values[2][1] * position.y + mat.values[2][2] * position.z + mat.values[2][3] * w;
				w          = mat.values[3][0] * position.x + mat.values[3][1] * position.y + mat.values[3][2] * position.z + mat.values[3][3] * w;
				
				//TODO: normal = ??

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