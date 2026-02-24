#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <iostream>

struct Vector3
{
	float x, y, z;

	Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {};
	
	bool operator==(const Vector3 other) const
	{
		return (x == other.x && y == other.y && z == other.z);
	}
};

inline std::ostream& operator<<(std::ostream& out_stream, const Vector3& vec)
{
	out_stream << "[" << vec.x << " ," << vec.y << ", " << vec.z << "]";
	return (out_stream);
};

#endif