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

	}
}