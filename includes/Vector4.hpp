#ifndef VECTOR4_HPP
#define VECTOR4_HPP

#include <iostream>

struct Vector4
{
	float x, y, z, w;

	Vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) : x(x), y(y), z(z), w(w) {};
};

std::ostream& operator<<(std::ostream& out_stream, const Vector4& vec)
{
	out_stream << "[" << vec.x << " ," << vec.y << ", " << vec.z << ", " << vec.w << "]";
	return (out_stream);
};

#endif