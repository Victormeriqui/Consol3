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

			constexpr Angle();
			constexpr Angle(float pitch, float yaw, float roll);

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