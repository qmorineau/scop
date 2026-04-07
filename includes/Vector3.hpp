#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <iostream>
#include <cmath>

struct Vector3
{
	float x, y, z;

	Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {};

	bool operator==(const Vector3& other) const
	{
		return (x == other.x && y == other.y && z == other.z);
	}

	Vector3 operator+(const Vector3& other) const
	{
		return (Vector3(x + other.x, y + other.y, z + other.z));
	}

	Vector3 operator-(const Vector3& other) const
	{
		return (Vector3(x - other.x, y - other.y, z - other.z));
	}

	Vector3 operator*(const Vector3& other) const
	{
		return (Vector3(x * other.x, y * other.y, z * other.z));
	}

	Vector3 operator*(const float s) const
	{
		return (Vector3(x * s, y * s, z * s));
	}

	Vector3 operator*=(const float s)
	{
		x *= s;
		y *= s;
		z *= s;
		return (*this);
	}

	Vector3 operator/(const float s) const
	{
		return (Vector3(x / s, y / s, z / s));
	}

	Vector3 operator+=(const Vector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return (*this);
	}

	Vector3 operator-=(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return (*this);
	}

	float lengthSquared() const
	{
		return x * x + y * y + z * z;
	}

	static Vector3 min(const Vector3& a, const Vector3& b)
	{
		return Vector3(
			std::min(a.x, b.x),
			std::min(a.y, b.y),
			std::min(a.z, b.z)
		);
	}

	static Vector3 max(const Vector3& a, const Vector3& b)
	{
		return Vector3(
			std::max(a.x, b.x),
			std::max(a.y, b.y),
			std::max(a.z, b.z)
		);
	}

	// To use Hash Table w/ vec3
	struct Vec3Hash {
		std::size_t operator()(const Vector3& v) const noexcept {
			std::size_t h1 = std::hash<float>{}(v.x);
			std::size_t h2 = std::hash<float>{}(v.y);
			std::size_t h3 = std::hash<float>{}(v.z);
			return h1 ^ (h2 << 1) ^ (h3 << 2);
		}
	};

	struct Vec3Eq {
		bool operator()(const Vector3& a, const Vector3& b) const noexcept {
			return a.x == b.x && a.y == b.y && a.z == b.z;
		}
	};
};

inline Vector3 operator*(float s, const Vector3& v)
{
	return (Vector3(v.x * s, v.y * s, v.z * s));
};

inline Vector3 operator*=(const float s, Vector3& v)
{
	return (Vector3(
		v.x * s,
		v.y * s,
		v.z * s
	));
}

inline std::ostream& operator<<(std::ostream& out_stream, const Vector3& vec)
{
	out_stream << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]";
	return (out_stream);
};

#endif