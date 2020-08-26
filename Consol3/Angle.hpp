#ifndef ANGLE_HPP
#define ANGLE_HPP

namespace Engine
{
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
			
			constexpr Angle& operator+=(const Angle& other) noexcept;
			constexpr Angle& operator-=(const Angle& other) noexcept;
			constexpr Angle& operator/=(const Angle& other) noexcept;
			constexpr Angle& operator*=(const Angle& other) noexcept;

			constexpr Angle& operator+=(float scalar) noexcept;
			constexpr Angle& operator-=(float scalar) noexcept;
			constexpr Angle& operator/=(float scalar) noexcept;
			constexpr Angle& operator*=(float scalar) noexcept;

			[[nodiscard]] constexpr Angle operator+(const Angle& other) const noexcept;
			[[nodiscard]] constexpr Angle operator-(const Angle& other) const noexcept;
			[[nodiscard]] constexpr Angle operator/(const Angle& other) const noexcept;
			[[nodiscard]] constexpr Angle operator*(const Angle& other) const noexcept;

			[[nodiscard]] constexpr Angle operator+(float scalar) const noexcept;
			[[nodiscard]] constexpr Angle operator-(float scalar) const noexcept;
			[[nodiscard]] constexpr Angle operator/(float scalar) const noexcept;
			[[nodiscard]] constexpr Angle operator*(float scalar) const noexcept;

			[[nodiscard]] constexpr bool operator==(const Angle& other) const noexcept;
		};
	}
}

#endif