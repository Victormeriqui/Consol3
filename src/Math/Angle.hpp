#ifndef ANGLE_HPP
#define ANGLE_HPP

namespace Math
{
	class Angle
	{
	public:
		float pitch;
		float yaw;
		float roll;

		constexpr Angle() : pitch(0), yaw(0), roll(0)
		{
		}

		constexpr Angle(float pitch, float yaw, float roll) : pitch(pitch), yaw(yaw), roll(roll)
		{
		}

		Angle& ToDegrees();
		Angle& ToRadians();

		[[nodiscard]] Angle GetInDegrees() const;
		[[nodiscard]] Angle GetInRadians() const;
			
		constexpr Angle& operator+=(const Angle& other) noexcept
		{
			pitch += other.pitch;
			yaw += other.yaw;
			roll += other.roll;

			return *this;
		}

		constexpr Angle& operator-=(const Angle& other) noexcept
		{
			pitch -= other.pitch;
			yaw -= other.yaw;
			roll -= other.roll;

			return *this;
		}

		constexpr Angle& operator/=(const Angle& other) noexcept
		{
			pitch /= other.pitch;
			yaw /= other.yaw;
			roll /= other.roll;

			return *this;
		}

		constexpr Angle& operator*=(const Angle& other) noexcept
		{
			pitch *= other.pitch;
			yaw *= other.yaw;
			roll *= other.roll;

			return *this;
		}

		constexpr Angle& operator+=(float scalar) noexcept
		{
			pitch += scalar;
			yaw += scalar;
			roll += scalar;

			return *this;
		}

		constexpr Angle& operator-=(float scalar) noexcept
		{
			pitch -= scalar;
			yaw -= scalar;
			roll -= scalar;

			return *this;
		}

		constexpr Angle& operator/=(float scalar) noexcept
		{
			pitch /= scalar;
			yaw /= scalar;
			roll /= scalar;

			return *this;
		}

		constexpr Angle& operator*=(float scalar) noexcept
		{
			pitch *= scalar;
			yaw *= scalar;
			roll *= scalar;

			return *this;
		}

		[[nodiscard]] constexpr Angle operator+(const Angle& other) const noexcept
		{
			return Angle(*this) += other;
		}

		[[nodiscard]] constexpr Angle operator-(const Angle& other) const noexcept
		{
			return Angle(*this) -= other;
		}

		[[nodiscard]] constexpr Angle operator/(const Angle& other) const noexcept
		{
			return Angle(*this) /= other;
		}

		[[nodiscard]] constexpr Angle operator*(const Angle& other) const noexcept
		{
			return Angle(*this) *= other;
		}

		[[nodiscard]] constexpr Angle operator+(float scalar) const noexcept
		{
			return Angle(*this) += scalar;
		}

		[[nodiscard]] constexpr Angle operator-(float scalar) const noexcept
		{
			return Angle(*this) -= scalar;
		}

		[[nodiscard]] constexpr Angle operator/(float scalar) const noexcept
		{
			return Angle(*this) /= scalar;
		}

		[[nodiscard]] constexpr Angle operator*(float scalar) const noexcept
		{
			return Angle(*this) *= scalar;
		}

		[[nodiscard]] constexpr bool operator==(const Angle& other) const noexcept
		{
			return (pitch == other.pitch && yaw == other.yaw && roll == other.roll);
		}
	};
}

#endif