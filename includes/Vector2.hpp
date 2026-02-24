#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <iostream>

struct Vector2
{
	float x, y;

	Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {};

	bool operator==(Vector2 other)
	{
		if (x == other.x && y == other.y)
			return true;
		return false;
	}
};

inline std::ostream& operator<<(std::ostream& out_stream, const Vector2& vec)
{
	out_stream << "[" << vec.x << " ," << vec.y << "]";
	return (out_stream);
};

#endif