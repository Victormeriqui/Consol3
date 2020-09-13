#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstdint>

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

			Matrix4& SetIdentity();

			constexpr Matrix4& operator*=(const Matrix4& other) noexcept
			{
				for (uint8_t y = 0; y < 4; y++)
				{
					for (uint8_t x = 0; x < 4; x++)
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

			[[nodiscard]] Matrix4 operator*(const Matrix4& other) const noexcept
			{
				return Matrix4(*this) *= other;
			}
		};

	}
}

#endif
