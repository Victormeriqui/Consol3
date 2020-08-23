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

			Matrix4();
			Matrix4(float values[4][4]);
			Matrix4(float value);

			constexpr Matrix4& operator*=(const Matrix4& other) noexcept
			{
				for (int y = 0; y < 4; y++)
				{
					for (int x = 0; x < 4; x++)
					{
						values[y][x] =
							values[y][0] * other.values[0][x] +
							values[y][1] * other.values[1][x] +
							values[y][2] * other.values[2][x] +
							values[y][3] * other.values[3][x];
					}
				}

				return *this;
			}

			[[nodiscard]] Matrix4 operator*(const Matrix4& other) const noexcept;

			[[nodiscard]] constexpr Vector3 operator*(const Vector3& vector) const
			{
				return Vector3(
					values[0][0] * vector.x + values[0][1] * vector.y + values[0][2] * vector.z + values[0][3] * vector.w,
					values[1][0] * vector.x + values[1][1] * vector.y + values[1][2] * vector.z + values[1][3] * vector.w,
					values[2][0] * vector.x + values[2][1] * vector.y + values[2][2] * vector.z + values[2][3] * vector.w,
					values[3][0] * vector.x + values[3][1] * vector.y + values[3][2] * vector.z + values[3][3] * vector.w);
			}

		};

	}
}

#endif
