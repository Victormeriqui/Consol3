#include "Matrix4.hpp"

#include "Vector3.hpp"

#include <algorithm>

namespace Engine
{
	namespace Math
	{



		constexpr Matrix4::Matrix4()
		{
		}

		constexpr Matrix4::Matrix4(float values[4][4])
		{
			std::copy(&values[0][0], &values[0][0] + 4*4, &this->values[0][0]);
		}

		constexpr Matrix4::Matrix4(float value)
		{
			std::fill(std::begin(values), std::begin(values) + 4*4, value);
		}

		constexpr Matrix4& Matrix4::operator*=(const Matrix4& other) noexcept
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

		[[nodiscard]] constexpr Matrix4 Matrix4::operator*(const Matrix4& other) const noexcept
		{
			return Matrix4(*this) * other;
		}

		[[nodiscard]] constexpr Vector3 Matrix4::operator*(const Vector3& vector) const noexcept
		{
			return Vector3(
				values[0][0] * vector.x + values[0][1] * vector.y + values[0][2] * vector.z + values[0][3] * vector.w,
				values[1][0] * vector.x + values[1][1] * vector.y + values[1][2] * vector.z + values[1][3] * vector.w,
				values[2][0] * vector.x + values[2][1] * vector.y + values[2][2] * vector.z + values[2][3] * vector.w,
				values[3][0] * vector.x + values[3][1] * vector.y + values[3][2] * vector.z + values[3][3] * vector.w);
		}
	}
}