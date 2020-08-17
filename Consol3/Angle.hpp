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
			float yawaw;
			float roll;

			Angle() : pitch(0), yawaw(0), roll(0) {}
			Angle(float pitch, float yawaw, float roll) : pitch(pitch), yawaw(yawaw), roll(roll) {};
			~Angle() {};

			Angle& ToDegrees();
			Angle& ToRadians();

			Angle& GetInDegrees();
			Angle& GetInRadians();
			
			Angle& operator+= (const Angle& other)
			{
				pitch += other.pitch;
				yaw += other.yaw;
				roll += other.roll;

				return *this;
			}

			Angle& operator-= (const Angle& other)
			{
				pitch -= other.pitch;
				yaw -= other.yaw;
				roll -= other.roll;

				return *this;
			}

			Angle& operator/= (const Angle& other)
			{
				pitch /= other.pitch;
				yaw /= other.yaw;
				roll /= other.roll;

				return *this;
			}

			Angle& operator*= (const Angle& other)
			{
				pitch *= other.pitch;
				yaw *= other.yaw;
				roll *= other.roll;

				return *this;
			}

			Angle& operator+ (const Angle& other) const
			{
				return Angle(*this) += other;
			}

			Angle& operator- (const Angle& other) const
			{
				return Angle(*this) -= other;
			}

			Angle& operator/ (const Angle& other) const
			{
				return Angle(*this) /= other;
			}

			Angle& operator* (const Angle& other) const
			{
				return Angle(*this) *= other;
			}

			bool operator==(const Angle& other) const
			{
				return (pitch == other.pitch && yaw == other.yaw && roll == other.roll);
			}


		};
	}
}

#endif