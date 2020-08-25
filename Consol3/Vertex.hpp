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
			Vector3 normal;

		public:
			constexpr Vertex(Vector3 position) : position(position), normal(Vector3(0, 0, 0)) {}
			constexpr Vertex(Vector3 position, Vector3 normal) : position(position), normal(normal) {};

			[[nodiscard]] const Vector3& GetPosition() const;
			Vertex& SetPosition(const Vector3& position);

			[[nodiscard]] const Vector3& GetNormal() const;
			Vertex& SetNormal(const Vector3& normal);


			Vertex& PerspectiveDivide();


			constexpr Vertex& operator*= (const Matrix4& mat) noexcept
			{
				position = mat * position;
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