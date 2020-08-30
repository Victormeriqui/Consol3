#include "Matrix4.hpp"

#include <algorithm>

namespace Engine
{
	namespace Math
	{
		Matrix4::Matrix4()
		{
			std::fill(&values[0][0], &values[0][0] + 4 * 4, 0.0f);
		}

		 Matrix4::Matrix4(float values[4][4])
		{
			std::copy(&values[0][0], &values[0][0] + 4*4, &this->values[0][0]);
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

	}
}