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
};

inline Vector3 operator*(float s, const Vector3& v)
{
	return (Vector3(v.x * s, v.y * s, v.z * s));
};

inline std::ostream& operator<<(std::ostream& out_stream, const Vector3& vec)
{
	out_stream << "[" << vec.x << " ," << vec.y << ", " << vec.z << "]";
	return (out_stream);
};

#endif