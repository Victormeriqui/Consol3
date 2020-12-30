#include "Matrix4.hpp"

#include "Util/MathUtil.hpp"

#include <algorithm>
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
		values[0][0] = 1;	values[0][1] = 0;	values[0][2] = 0;	values[0][3] = 0;
		values[1][0] = 0;	values[1][1] = 1;	values[1][2] = 0;	values[1][3] = 0;
		values[2][0] = 0;	values[2][1] = 0;	values[2][2] = 1;	values[2][3] = 0;
		values[3][0] = 0;	values[3][1] = 0;	values[3][2] = 0;	values[3][3] = 1;

		return *this;
	}

	Matrix4& Matrix4::SetPerspective(uint16_t width, uint16_t height, float znear, float zfar, float fov)
	{
		// aspect ratio
		float ar = (float)width / (float)height;
		float fov_rad = Util::ToRadians(fov);
		float tanhf = std::tan(fov_rad / 2.0f);
		float zrange = znear - zfar;

		values[0][0] = 1.0f / (tanhf * ar); values[0][1] = 0;            values[0][2] = 0;                        values[0][3] = 0;
		values[1][0] = 0;                   values[1][1] = 1.0f / tanhf; values[1][2] = 0;                        values[1][3] = 0;
		values[2][0] = 0;                   values[2][1] = 0;            values[2][2] = (-znear - zfar) / zrange; values[2][3] = (2.0f * zfar * znear) / zrange;
		values[3][0] = 0;                   values[3][1] = 0;            values[3][2] = 1;                        values[3][3] = 0;

		return *this;
	}

	Matrix4& Matrix4::SetOrthographic(float left, float right, float up, float down, float near, float far)
	{
		float width = right - left;
		float height = up - down;
		float depth = far - near;

		values[0][0] = 2.0f / width; values[0][1] = 0;             values[0][2] = 0;             values[0][3] = -(right + left) / width;
		values[1][0] = 0;            values[1][1] = 2.0f / height; values[1][2] = 0;             values[1][3] = -(up + down) / height;
		values[2][0] = 0;            values[2][1] = 0;             values[2][2] = -2.0f / depth; values[2][3] = -(far + near) / depth;
		values[3][0] = 0;            values[3][1] = 0;             values[3][2] = 0;             values[3][3] = 1;

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

		inv_mat[0][0] = values[1][1] * values[2][2] * values[3][3] -
			values[1][1] * values[2][3] * values[3][2] -
			values[2][1] * values[1][2] * values[3][3] +
			values[2][1] * values[1][3] * values[3][2] +
			values[3][1] * values[1][2] * values[2][3] -
			values[3][1] * values[1][3] * values[2][2];

		inv_mat[1][0] = -values[1][0] * values[2][2] * values[3][3] +
			values[1][0] * values[2][3] * values[3][2] +
			values[2][0] * values[1][2] * values[3][3] -
			values[2][0] * values[1][3] * values[3][2] -
			values[3][0] * values[1][2] * values[2][3] +
			values[3][0] * values[1][3] * values[2][2];

		inv_mat[2][0] = values[1][0] * values[2][1] * values[3][3] -
			values[1][0] * values[2][3] * values[3][1] -
			values[2][0] * values[1][1] * values[3][3] +
			values[2][0] * values[1][3] * values[3][1] +
			values[3][0] * values[1][1] * values[2][3] -
			values[3][0] * values[1][3] * values[2][1];

		inv_mat[3][0] = -values[1][0] * values[2][1] * values[3][2] +
			values[1][0] * values[2][2] * values[3][1] +
			values[2][0] * values[1][1] * values[3][2] -
			values[2][0] * values[1][2] * values[3][1] -
			values[3][0] * values[1][1] * values[2][2] +
			values[3][0] * values[1][2] * values[2][1];

		inv_mat[0][1] = -values[0][1] * values[2][2] * values[3][3] +
			values[0][1] * values[2][3] * values[3][2] +
			values[2][1] * values[0][2] * values[3][3] -
			values[2][1] * values[0][3] * values[3][2] -
			values[3][1] * values[0][2] * values[2][3] +
			values[3][1] * values[0][3] * values[2][2];

		inv_mat[1][1] = values[0][0] * values[2][2] * values[3][3] -
			values[0][0] * values[2][3] * values[3][2] -
			values[2][0] * values[0][2] * values[3][3] +
			values[2][0] * values[0][3] * values[3][2] +
			values[3][0] * values[0][2] * values[2][3] -
			values[3][0] * values[0][3] * values[2][2];

		inv_mat[2][1] = -values[0][0] * values[2][1] * values[3][3] +
			values[0][0] * values[2][3] * values[3][1] +
			values[2][0] * values[0][1] * values[3][3] -
			values[2][0] * values[0][3] * values[3][1] -
			values[3][0] * values[0][1] * values[2][3] +
			values[3][0] * values[0][3] * values[2][1];

		inv_mat[3][1] = values[0][0] * values[2][1] * values[3][2] -
			values[0][0] * values[2][2] * values[3][1] -
			values[2][0] * values[0][1] * values[3][2] +
			values[2][0] * values[0][2] * values[3][1] +
			values[3][0] * values[0][1] * values[2][2] -
			values[3][0] * values[0][2] * values[2][1];

		inv_mat[0][2] = values[0][1] * values[1][2] * values[3][3] -
			values[0][1] * values[1][3] * values[3][2] -
			values[1][1] * values[0][2] * values[3][3] +
			values[1][1] * values[0][3] * values[3][2] +
			values[3][1] * values[0][2] * values[1][3] -
			values[3][1] * values[0][3] * values[1][2];

		inv_mat[1][2] = -values[0][0] * values[1][2] * values[3][3] +
			values[0][0] * values[1][3] * values[3][2] +
			values[1][0] * values[0][2] * values[3][3] -
			values[1][0] * values[0][3] * values[3][2] -
			values[3][0] * values[0][2] * values[1][3] +
			values[3][0] * values[0][3] * values[1][2];

		inv_mat[2][2] = values[0][0] * values[1][1] * values[3][3] -
			values[0][0] * values[1][3] * values[3][1] -
			values[1][0] * values[0][1] * values[3][3] +
			values[1][0] * values[0][3] * values[3][1] +
			values[3][0] * values[0][1] * values[1][3] -
			values[3][0] * values[0][3] * values[1][1];

		inv_mat[3][2] = -values[0][0] * values[1][1] * values[3][2] +
			values[0][0] * values[1][2] * values[3][1] +
			values[1][0] * values[0][1] * values[3][2] -
			values[1][0] * values[0][2] * values[3][1] -
			values[3][0] * values[0][1] * values[1][2] +
			values[3][0] * values[0][2] * values[1][1];

		inv_mat[0][3] = -values[0][1] * values[1][2] * values[2][3] +
			values[0][1] * values[1][3] * values[2][2] +
			values[1][1] * values[0][2] * values[2][3] -
			values[1][1] * values[0][3] * values[2][2] -
			values[2][1] * values[0][2] * values[1][3] +
			values[2][1] * values[0][3] * values[1][2];

		inv_mat[1][3] = values[0][0] * values[1][2] * values[2][3] -
			values[0][0] * values[1][3] * values[2][2] -
			values[1][0] * values[0][2] * values[2][3] +
			values[1][0] * values[0][3] * values[2][2] +
			values[2][0] * values[0][2] * values[1][3] -
			values[2][0] * values[0][3] * values[1][2];

		inv_mat[2][3] = -values[0][0] * values[1][1] * values[2][3] +
			values[0][0] * values[1][3] * values[2][1] +
			values[1][0] * values[0][1] * values[2][3] -
			values[1][0] * values[0][3] * values[2][1] -
			values[2][0] * values[0][1] * values[1][3] +
			values[2][0] * values[0][3] * values[1][1];

		inv_mat[3][3] = values[0][0] * values[1][1] * values[2][2] -
			values[0][0] * values[1][2] * values[2][1] -
			values[1][0] * values[0][1] * values[2][2] +
			values[1][0] * values[0][2] * values[2][1] +
			values[2][0] * values[0][1] * values[1][2] -
			values[2][0] * values[0][2] * values[1][1];

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