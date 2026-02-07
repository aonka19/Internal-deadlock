#pragma once
#include <cmath>
#include <string>

constexpr auto PI = 3.1415927f;

class Vector2 {
public:
    float x{}, y{};
    Vector2() = default;
    Vector2(float x, float y) : x(x), y(y) {}
};

class Vector3 {
public:
    float x{}, y{}, z{};

    Vector3() = default;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3& rhs) const { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
    Vector3 operator-(const Vector3& rhs) const { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
    Vector3 operator*(float rhs) const { return { x * rhs, y * rhs, z * rhs }; }
    Vector3 operator/(float rhs) const { return { x / rhs, y / rhs, z / rhs }; }

    float Length() const { return std::sqrt(x * x + y * y + z * z); }
    float Distance(const Vector3& rhs) const { return (*this - rhs).Length(); }

    void NormalizeAngle(float yawMin = -180.f, float yawMax = 180.f,
        float pitchMin = -89.f, float pitchMax = 89.f);
};

class Vector4 {
public:
    float x{}, y{}, z{}, w{};
    Vector4() = default;
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
};

using Vec3 = Vector3;
using Vec = Vector3;

float DegreesToRadians(float num);
float RadiansToDegrees(float num);
Vec3 CalcAngle(const Vec3& origin, const Vec3& target, bool invertYaw = false, bool invertPitch = false);
