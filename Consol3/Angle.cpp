#include "Angle.hpp"

#include "MathUtil.hpp"

namespace Engine
{
	namespace Math
	{
		constexpr Angle::Angle() : pitch(0), yaw(0), roll(0)
		{
		}
		
		constexpr Angle::Angle(float pitch, float yaw, float roll) : pitch(pitch), yaw(yaw), roll(roll)
		{
		}

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

		[[nodiscard]] Angle Angle::GetInDegrees() const
		{
			return Angle(*this).ToDegrees();
		}

		[[nodiscard]] Angle Angle::GetInRadians() const
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

		[[nodiscard]] constexpr Angle Angle::operator+(const Angle& other) const noexcept
		{
			return Angle(*this) += other;
		}

		[[nodiscard]] constexpr Angle Angle::operator-(const Angle& other) const noexcept
		{
			return Angle(*this) -= other;
		}

		[[nodiscard]] constexpr Angle Angle::operator/(const Angle& other) const noexcept
		{
			return Angle(*this) /= other;
		}

		[[nodiscard]] constexpr Angle Angle::operator*(const Angle& other) const noexcept
		{
			return Angle(*this) *= other;
		}

		[[nodiscard]] constexpr Angle Angle::operator+(float scalar) const noexcept
		{
			return Angle(*this) += scalar;
		}

		[[nodiscard]] constexpr Angle Angle::operator-(float scalar) const noexcept
		{
			return Angle(*this) -= scalar;
		}

		[[nodiscard]] constexpr Angle Angle::operator/(float scalar) const noexcept
		{
			return Angle(*this) /= scalar;
		}

		[[nodiscard]] constexpr Angle Angle::operator*(float scalar) const noexcept
		{
			return Angle(*this) *= scalar;
		}

		[[nodiscard]] constexpr bool Angle::operator==(const Angle& other) const noexcept
		{
			return (pitch == other.pitch && yaw == other.yaw && roll == other.roll);
		}

	}
}
