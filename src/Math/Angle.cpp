#include "Angle.hpp"

#include "Util/MathUtil.hpp"

namespace Math
{
	Angle& Angle::ToDegrees()
	{
		pitch = Util::ToDegrees(pitch);
		yaw	  = Util::ToDegrees(pitch);
		roll  = Util::ToDegrees(pitch);

		return *this;
	}

	Angle& Angle::ToRadians()
	{
		pitch = Util::ToRadians(pitch);
		yaw	  = Util::ToRadians(pitch);
		roll  = Util::ToRadians(pitch);

		return *this;
	}

	Angle Angle::GetInDegrees() const
	{
		return Angle(*this).ToDegrees();
	}

	Angle Angle::GetInRadians() const
	{
		return Angle(*this).ToRadians();
	}
}
