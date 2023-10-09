#include "Matrix4.hpp"
#include "Quaternion.hpp"
#include "Vector3.hpp"

#include "Util/MathUtil.hpp"

#include <cmath>

namespace Math
{
	Matrix4::Matrix4()
	{
		std::fill(&values[0][0], &values[0][0] + 4 * 4, 0.0f);
	}

	Matrix4::Matrix4(float values[4][4])
	{
		std::copy(&values[0][0], &values[0][0] + 4 * 4, &this->values[0][0]);
	}

	Matrix4::Matrix4(float value)
	{
		std::fill(&values[0][0], &values[0][0] + 4 * 4, value);
	}

	Matrix4& Matrix4::SetIdentity()
	{
		values[0][0] = 1;
		values[0][1] = 0;
		values[0][2] = 0;
		values[0][3] = 0;
		values[1][0] = 0;
		values[1][1] = 1;
		values[1][2] = 0;
		values[1][3] = 0;
		values[2][0] = 0;
		values[2][1] = 0;
		values[2][2] = 1;
		values[2][3] = 0;
		values[3][0] = 0;
		values[3][1] = 0;
		values[3][2] = 0;
		values[3][3] = 1;

		return *this;
	}

	Matrix4& Matrix4::SetTranslation(const Vector3& translation)
	{
		values[0][0] = 1;
		values[0][1] = 0;
		values[0][2] = 0;
		values[0][3] = translation.x;
		values[1][0] = 0;
		values[1][1] = 1;
		values[1][2] = 0;
		values[1][3] = translation.y;
		values[2][0] = 0;
		values[2][1] = 0;
		values[2][2] = 1;
		values[2][3] = translation.z;
		values[3][0] = 0;
		values[3][1] = 0;
		values[3][2] = 0;
		values[3][3] = 1;

		return *this;
	}

	Matrix4& Matrix4::SetScale(const Vector3& scale)
	{
		values[0][0] = scale.x;
		values[0][1] = 0;
		values[0][2] = 0;
		values[0][3] = 0;
		values[1][0] = 0;
		values[1][1] = scale.y;
		values[1][2] = 0;
		values[1][3] = 0;
		values[2][0] = 0;
		values[2][1] = 0;
		values[2][2] = scale.z;
		values[2][3] = 0;
		values[3][0] = 0;
		values[3][1] = 0;
		values[3][2] = 0;
		values[3][3] = 1;

		return *this;
	}

	Matrix4& Matrix4::SetDirectionalRotation(const Vector3& right, const Vector3& up, const Vector3& forward)
	{
		values[0][0] = right.x;
		values[0][1] = right.y;
		values[0][2] = right.z;
		values[0][3] = 0;
		values[1][0] = up.x;
		values[1][1] = up.y;
		values[1][2] = up.z;
		values[1][3] = 0;
		values[2][0] = forward.x;
		values[2][1] = forward.y;
		values[2][2] = forward.z;
		values[2][3] = 0;
		values[3][0] = 0;
		values[3][1] = 0;
		values[3][2] = 0;
		values[3][3] = 1;

		return *this;
	}

	Matrix4& Matrix4::SetQuaternionRotation(const Quaternion& rotation)
	{
		float xx = rotation.x * rotation.x;
		float yy = rotation.y * rotation.y;
		float zz = rotation.z * rotation.z;
		float ww = rotation.w * rotation.w;
		float x2 = rotation.x * 2.0f;
		float y2 = rotation.y * 2.0f;
		//float z2 = rotation.z * 2.0f;
		float w2 = rotation.w * 2.0f;
		float xy = x2 * rotation.y;
		float xz = x2 * rotation.z;
		float yz = y2 * rotation.z;
		float wx = w2 * rotation.x;
		float wy = w2 * rotation.y;
		float wz = w2 * rotation.z;

		values[0][0] = ww + xx - yy - zz;
		values[0][1] = xy - wz;
		values[0][2] = xz + wy;
		values[0][3] = 0;
		values[1][0] = xy + wz;
		values[1][1] = ww - xx + yy - zz;
		values[1][2] = yz - wx;
		values[1][3] = 0;
		values[2][0] = xz - wy;
		values[2][1] = yz + wx;
		values[2][2] = ww - xx - yy + zz;
		values[2][3] = 0;
		values[3][0] = 0;
		values[3][1] = 0;
		values[3][2] = 0;
		values[3][3] = 1;

		return *this;
	}

	Matrix4& Matrix4::SetPerspectiveProjection(uint16_t width, uint16_t height, float znear, float zfar, float fov)
	{
		// aspect ratio
		float ar	  = (float)width / (float)height;
		float fov_rad = Util::ToRadians(fov);
		float tanhf	  = std::tan(fov_rad / 2.0f);
		float zrange  = znear - zfar;

		values[0][0] = 1.0f / (tanhf * ar);
		values[0][1] = 0;
		values[0][2] = 0;
		values[0][3] = 0;
		values[1][0] = 0;
		values[1][1] = 1.0f / tanhf;
		values[1][2] = 0;
		values[1][3] = 0;
		values[2][0] = 0;
		values[2][1] = 0;
		values[2][2] = (-znear - zfar) / zrange;
		values[2][3] = (2.0f * zfar * znear) / zrange;
		values[3][0] = 0;
		values[3][1] = 0;
		values[3][2] = 1;
		values[3][3] = 0;

		return *this;
	}

	Matrix4& Matrix4::SetOrthographicProjection(float left, float right, float up, float down, float near, float far)
	{
		float width	 = right - left;
		float height = down - up;
		float depth	 = far - near;

		values[0][0] = 2.0f / width;
		values[0][1] = 0;
		values[0][2] = 0;
		values[0][3] = -(right + left) / width;
		values[1][0] = 0;
		values[1][1] = 2.0f / height;
		values[1][2] = 0;
		values[1][3] = -(up + down) / height;
		values[2][0] = 0;
		values[2][1] = 0;
		values[2][2] = 2.0f / depth;
		values[2][3] = -(far + near) / depth;
		values[3][0] = 0;
		values[3][1] = 0;
		values[3][2] = 0;
		values[3][3] = 1;

		return *this;
	}

	Matrix4& Matrix4::SetViewportMatrix(uint16_t width, uint16_t height)
	{
		float width_h  = width / 2.0f;
		float height_h = height / 2.0f;

		values[0][0] = width_h;
		values[0][1] = 0;
		values[0][2] = 0;
		values[0][3] = width_h;
		values[1][0] = 0;
		values[1][1] = -height_h;
		values[1][2] = 0;
		values[1][3] = height_h;
		values[2][0] = 0;
		values[2][1] = 0;
		values[2][2] = 1;
		values[2][3] = 0;
		values[3][0] = 0;
		values[3][1] = 0;
		values[3][2] = 0;
		values[3][3] = 1;

		return *this;
	}

	Matrix4& Matrix4::SetTBNMatrix(const Vector3& tangent, const Vector3& bitangent, const Vector3 normal)
	{
		values[0][0] = tangent.x;
		values[0][1] = tangent.y;
		values[0][2] = tangent.z;
		values[0][3] = 0;
		values[1][0] = bitangent.x;
		values[1][1] = bitangent.y;
		values[1][2] = bitangent.z;
		values[1][3] = 0;
		values[2][0] = normal.x;
		values[2][1] = normal.y;
		values[2][2] = normal.z;
		values[2][3] = 0;
		values[3][0] = 0;
		values[3][1] = 0;
		values[3][2] = 0;
		values[3][3] = 1;

		// values[0][0] = tangent.x;
		// values[1][0] = tangent.y;
		// values[2][0] = tangent.z;
		// values[3][0] = 0;
		// values[0][1] = bitangent.x;
		// values[1][1] = bitangent.y;
		// values[2][1] = bitangent.z;
		// values[3][1] = 0;
		// values[0][2] = normal.x;
		// values[1][2] = normal.y;
		// values[2][2] = normal.z;
		// values[3][2] = 0;
		// values[0][3] = 0;
		// values[1][3] = 0;
		// values[2][3] = 0;
		// values[3][3] = 1;

		return *this;
	}

	Matrix4& Matrix4::Transpose()
	{
		for (uint8_t y = 0; y < 4; y++)
		{
			for (uint8_t x = 0; x < 4; x++)
				values[y][x] = values[x][y];
		}

		return *this;
	}

	// http://www.mesa3d.org/
	Matrix4& Matrix4::Invert()
	{
		float inv_mat[4][4];
		float determinant;

		inv_mat[0][0] = values[1][1] * values[2][2] * values[3][3] - values[1][1] * values[2][3] * values[3][2]
						- values[2][1] * values[1][2] * values[3][3] + values[2][1] * values[1][3] * values[3][2]
						+ values[3][1] * values[1][2] * values[2][3] - values[3][1] * values[1][3] * values[2][2];

		inv_mat[1][0] = -values[1][0] * values[2][2] * values[3][3] + values[1][0] * values[2][3] * values[3][2]
						+ values[2][0] * values[1][2] * values[3][3] - values[2][0] * values[1][3] * values[3][2]
						- values[3][0] * values[1][2] * values[2][3] + values[3][0] * values[1][3] * values[2][2];

		inv_mat[2][0] = values[1][0] * values[2][1] * values[3][3] - values[1][0] * values[2][3] * values[3][1]
						- values[2][0] * values[1][1] * values[3][3] + values[2][0] * values[1][3] * values[3][1]
						+ values[3][0] * values[1][1] * values[2][3] - values[3][0] * values[1][3] * values[2][1];

		inv_mat[3][0] = -values[1][0] * values[2][1] * values[3][2] + values[1][0] * values[2][2] * values[3][1]
						+ values[2][0] * values[1][1] * values[3][2] - values[2][0] * values[1][2] * values[3][1]
						- values[3][0] * values[1][1] * values[2][2] + values[3][0] * values[1][2] * values[2][1];

		inv_mat[0][1] = -values[0][1] * values[2][2] * values[3][3] + values[0][1] * values[2][3] * values[3][2]
						+ values[2][1] * values[0][2] * values[3][3] - values[2][1] * values[0][3] * values[3][2]
						- values[3][1] * values[0][2] * values[2][3] + values[3][1] * values[0][3] * values[2][2];

		inv_mat[1][1] = values[0][0] * values[2][2] * values[3][3] - values[0][0] * values[2][3] * values[3][2]
						- values[2][0] * values[0][2] * values[3][3] + values[2][0] * values[0][3] * values[3][2]
						+ values[3][0] * values[0][2] * values[2][3] - values[3][0] * values[0][3] * values[2][2];

		inv_mat[2][1] = -values[0][0] * values[2][1] * values[3][3] + values[0][0] * values[2][3] * values[3][1]
						+ values[2][0] * values[0][1] * values[3][3] - values[2][0] * values[0][3] * values[3][1]
						- values[3][0] * values[0][1] * values[2][3] + values[3][0] * values[0][3] * values[2][1];

		inv_mat[3][1] = values[0][0] * values[2][1] * values[3][2] - values[0][0] * values[2][2] * values[3][1]
						- values[2][0] * values[0][1] * values[3][2] + values[2][0] * values[0][2] * values[3][1]
						+ values[3][0] * values[0][1] * values[2][2] - values[3][0] * values[0][2] * values[2][1];

		inv_mat[0][2] = values[0][1] * values[1][2] * values[3][3] - values[0][1] * values[1][3] * values[3][2]
						- values[1][1] * values[0][2] * values[3][3] + values[1][1] * values[0][3] * values[3][2]
						+ values[3][1] * values[0][2] * values[1][3] - values[3][1] * values[0][3] * values[1][2];

		inv_mat[1][2] = -values[0][0] * values[1][2] * values[3][3] + values[0][0] * values[1][3] * values[3][2]
						+ values[1][0] * values[0][2] * values[3][3] - values[1][0] * values[0][3] * values[3][2]
						- values[3][0] * values[0][2] * values[1][3] + values[3][0] * values[0][3] * values[1][2];

		inv_mat[2][2] = values[0][0] * values[1][1] * values[3][3] - values[0][0] * values[1][3] * values[3][1]
						- values[1][0] * values[0][1] * values[3][3] + values[1][0] * values[0][3] * values[3][1]
						+ values[3][0] * values[0][1] * values[1][3] - values[3][0] * values[0][3] * values[1][1];

		inv_mat[3][2] = -values[0][0] * values[1][1] * values[3][2] + values[0][0] * values[1][2] * values[3][1]
						+ values[1][0] * values[0][1] * values[3][2] - values[1][0] * values[0][2] * values[3][1]
						- values[3][0] * values[0][1] * values[1][2] + values[3][0] * values[0][2] * values[1][1];

		inv_mat[0][3] = -values[0][1] * values[1][2] * values[2][3] + values[0][1] * values[1][3] * values[2][2]
						+ values[1][1] * values[0][2] * values[2][3] - values[1][1] * values[0][3] * values[2][2]
						- values[2][1] * values[0][2] * values[1][3] + values[2][1] * values[0][3] * values[1][2];

		inv_mat[1][3] = values[0][0] * values[1][2] * values[2][3] - values[0][0] * values[1][3] * values[2][2]
						- values[1][0] * values[0][2] * values[2][3] + values[1][0] * values[0][3] * values[2][2]
						+ values[2][0] * values[0][2] * values[1][3] - values[2][0] * values[0][3] * values[1][2];

		inv_mat[2][3] = -values[0][0] * values[1][1] * values[2][3] + values[0][0] * values[1][3] * values[2][1]
						+ values[1][0] * values[0][1] * values[2][3] - values[1][0] * values[0][3] * values[2][1]
						- values[2][0] * values[0][1] * values[1][3] + values[2][0] * values[0][3] * values[1][1];

		inv_mat[3][3] = values[0][0] * values[1][1] * values[2][2] - values[0][0] * values[1][2] * values[2][1]
						- values[1][0] * values[0][1] * values[2][2] + values[1][0] * values[0][2] * values[2][1]
						+ values[2][0] * values[0][1] * values[1][2] - values[2][0] * values[0][2] * values[1][1];

		determinant = values[0][0] * inv_mat[0][0] + values[0][1] * inv_mat[1][0] + values[0][2] * inv_mat[2][0] + values[0][3] * inv_mat[3][0];

		// matrix isn't invertible, just return the same matrix
		if (determinant == 0)
			return *this;

		determinant = 1.0f / determinant;

		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				values[y][x] = inv_mat[y][x] * determinant;
			}
		}

		return *this;
	}

	Matrix4 Matrix4::GetInverted()
	{
		return Matrix4(*this).Invert();
	}

	Matrix4 Matrix4::GetTransposed()
	{
		return Matrix4(*this).Transpose();
	}
}
