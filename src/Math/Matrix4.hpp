#ifndef MATRIX4_HPP
#define MATRIX4_HPP

#include <algorithm>
#include <cstdint>

namespace Math
{
	class Quaternion;
	class Vector3;

	class Matrix4
	{
	public:
		float values[4][4];

		Matrix4();
		Matrix4(float values[4][4]);
		Matrix4(float value);

		Matrix4& SetIdentity();
		Matrix4& SetTranslation(const Vector3& translation);
		Matrix4& SetScale(const Vector3& scale);
		Matrix4& SetDirectionalRotation(const Vector3& right, const Vector3& up, const Vector3& forward);
		Matrix4& SetQuaternionRotation(const Quaternion& rotation);
		Matrix4& SetPerspectiveProjection(uint16_t width, uint16_t height, float znear, float zfar, float fov);
		Matrix4& SetOrthographicProjection(float left, float right, float up, float down, float near, float far);
		Matrix4& SetViewportMatrix(uint16_t width, uint16_t height);
		Matrix4& SetTBNMatrix(const Vector3& tangent, const Vector3& bitangent, const Vector3 normal);

		Matrix4& Invert();
		Matrix4& Transpose();

		Matrix4 GetInverted();
		Matrix4 GetTransposed();

		[[nodiscard]] Matrix4 operator*(const Matrix4& other) const noexcept
		{
			float new_values[4][4];

			for (uint8_t y = 0; y < 4; y++)
			{
				for (uint8_t x = 0; x < 4; x++)
				{
					new_values[y][x] = values[y][0] * other.values[0][x] + values[y][1] * other.values[1][x] + values[y][2] * other.values[2][x]
									   + values[y][3] * other.values[3][x];
				}
			}

			return Matrix4(new_values);
		}

		Matrix4& operator*=(const Matrix4& other) noexcept
		{
			Matrix4 new_mat = *this * other;

			std::copy(&new_mat.values[0][0], &new_mat.values[0][0] + 4 * 4, &this->values[0][0]);

			return *this;
		}
	};
}

#endif
