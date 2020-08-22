#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "Vector3.hpp"

namespace Engine
{
	namespace Math
	{
		class Matrix4
		{

		public:
			float values[4][4];

			constexpr Matrix4();
			constexpr Matrix4(float values[4][4]);
			constexpr Matrix4(float value);

			constexpr Matrix4& operator*=(const Matrix4& other) noexcept;
			[[nodiscard]] constexpr Matrix4 operator*(const Matrix4& other) const noexcept;
			[[nodiscard]] constexpr Vector3 operator*(const Vector3& vector) const noexcept;
		};

	}
}

#endif
