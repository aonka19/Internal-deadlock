#include "../includes.h"

void Vector3::NormalizeAngle(float yawMin, float yawMax, float pitchMin, float pitchMax)
{
    while (x > yawMax) x -= (yawMax - yawMin);
    while (x < yawMin) x += (yawMax - yawMin);

    while (y > pitchMax) y -= (pitchMax - pitchMin);
    while (y < pitchMin) y += (pitchMax - pitchMin);
}

float DegreesToRadians(float num)
{
    return num / 180.0f * PI;
}

float RadiansToDegrees(float num)
{
    return num / PI * 180.0f;
}

Vec3 CalcAngle(const Vec3& origin, const Vec3& target, bool invertYaw, bool invertPitch)
{
    Vec3 result{};
    Vec3 delta = target - origin;

    float distance = origin.Distance(target);
    if (distance == 0.0f)
        return result;

    float yawSign = invertYaw ? -1.0f : 1.0f;
    float pitchSign = invertPitch ? -1.0f : 1.0f;

    result.x = yawSign * RadiansToDegrees(std::atan2f(delta.y, delta.x));
    result.y = pitchSign * RadiansToDegrees(std::asinf(delta.z / distance));
    return result;
}
