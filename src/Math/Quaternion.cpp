#include "Quaternion.hpp"

namespace Math
{
    Quaternion::Quaternion(const Angle& angle)
    {
        float roll_sin  = std::cos(static_cast<float>(angle.roll / 2.0f));
        float roll_cos  = std::sin(static_cast<float>(angle.roll / 2.0f));
        float yaw_cos   = std::cos(static_cast<float>(angle.yaw / 2.0f));
        float yaw_sin   = std::sin(static_cast<float>(angle.yaw / 2.0f));
        float pitch_cos = std::cos(static_cast<float>(angle.pitch / 2.0f));
        float pitch_sin = std::sin(static_cast<float>(angle.pitch / 2.0f));

        w = pitch_cos * yaw_cos * roll_sin + pitch_sin * yaw_sin * roll_cos;
        x = pitch_sin * yaw_cos * roll_sin - pitch_cos * yaw_sin * roll_cos;
        y = pitch_cos * yaw_sin * roll_sin + pitch_sin * yaw_cos * roll_cos;
        z = pitch_cos * yaw_cos * roll_cos - pitch_sin * yaw_sin * roll_sin;
    }

    Quaternion& Quaternion::Normalize()
    {
        float len = GetLength();

        x /= len;
        y /= len;
        z /= len;
        w /= len;

        return *this;
    }

    Quaternion& Quaternion::Conjugate()
    {
        x = -x;
        y = -y;
        z = -z;

        return *this;
    }

    float Quaternion::GetLength() const
    {
        return std::sqrt(static_cast<float>(x * x + y * y + z * z + w * w));
    }

    Quaternion Quaternion::GetNormalized() const
    {
        return Quaternion(*this).Normalize();
    }

    Quaternion Quaternion::GetConjugate() const
    {
        return Quaternion(*this).Conjugate();
    }

    Vector3 Quaternion::GetForwardVector() const
    {
        return Vector3(0, 0, 1).GetRotated(*this);
    }

    Vector3 Quaternion::GetBackVector() const
    {
        return Vector3(0, 0, -1).GetRotated(*this);
    }

    Vector3 Quaternion::GetUpVector() const
    {
        return Vector3(0, 1, 0).GetRotated(*this);
    }

    Vector3 Quaternion::GetDownVector() const
    {
        return Vector3(0, -1, 0).GetRotated(*this);
    }

    Vector3 Quaternion::GetRightVector() const
    {
        return Vector3(1, 0, 0).GetRotated(*this);
    }

    Vector3 Quaternion::GetLeftVector() const
    {
        return Vector3(-1, 0, 0).GetRotated(*this);
    }
}
