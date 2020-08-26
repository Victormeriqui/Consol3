#include "Angle.hpp"

#include "MathUtil.hpp"

namespace Engine
{
	namespace Math
	{
		Angle& Angle::ToDegrees()
		{
			pitch = Util::ToDegrees(pitch);
			yaw = Util::ToDegrees(pitch);
			roll = Util::ToDegrees(pitch);
			
			return *this;
		}

		Angle& Angle::ToRadians()
		{
			pitch = Util::ToRadians(pitch);
			yaw = Util::ToRadians(pitch);
			roll = Util::ToRadians(pitch);

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

		constexpr Angle& Angle::operator+=(const Angle& other) noexcept
		{
			pitch += other.pitch;
			yaw += other.yaw;
			roll += other.roll;

			return *this;
		}

		constexpr Angle& Angle::operator-=(const Angle& other) noexcept
		{
			pitch -= other.pitch;
			yaw -= other.yaw;
			roll -= other.roll;

			return *this;
		}

		constexpr Angle& Angle::operator/=(const Angle& other) noexcept
		{
			pitch /= other.pitch;
			yaw /= other.yaw;
			roll /= other.roll;

			return *this;
		}

		constexpr Angle& Angle::operator*=(const Angle& other) noexcept
		{
			pitch *= other.pitch;
			yaw *= other.yaw;
			roll *= other.roll;

			return *this;
		}

		constexpr Angle& Angle::operator+=(float scalar) noexcept
		{
			pitch += scalar;
			yaw += scalar;
			roll += scalar;

			return *this;
		}

		constexpr Angle& Angle::operator-=(float scalar) noexcept
		{
			pitch -= scalar;
			yaw -= scalar;
			roll -= scalar;

			return *this;
		}

		constexpr Angle& Angle::operator/=(float scalar) noexcept
		{
			pitch /= scalar;
			yaw /= scalar;
			roll /= scalar;

			return *this;
		}

		constexpr Angle& Angle::operator*=(float scalar) noexcept
		{
			pitch *= scalar;
			yaw *= scalar;
			roll *= scalar;

			return *this;
		}

		constexpr Angle Angle::operator+(const Angle& other) const noexcept
		{
			return Angle(*this) += other;
		}

		constexpr Angle Angle::operator-(const Angle& other) const noexcept
		{
			return Angle(*this) -= other;
		}

		constexpr Angle Angle::operator/(const Angle& other) const noexcept
		{
			return Angle(*this) /= other;
		}

		constexpr Angle Angle::operator*(const Angle& other) const noexcept
		{
			return Angle(*this) *= other;
		}

		constexpr Angle Angle::operator+(float scalar) const noexcept
		{
			return Angle(*this) += scalar;
		}

		constexpr Angle Angle::operator-(float scalar) const noexcept
		{
			return Angle(*this) -= scalar;
		}

		constexpr Angle Angle::operator/(float scalar) const noexcept
		{
			return Angle(*this) /= scalar;
		}

		constexpr Angle Angle::operator*(float scalar) const noexcept
		{
			return Angle(*this) *= scalar;
		}

		constexpr bool Angle::operator==(const Angle& other) const noexcept
		{
			return (pitch == other.pitch && yaw == other.yaw && roll == other.roll);
		}

	}
}
