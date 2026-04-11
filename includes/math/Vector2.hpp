#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <iostream>

struct Vector2
{
	float x, y;

	Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {};

	bool operator==(const Vector2 other) const
	{
		return (x == other.x && y == other.y);
	}

	Vector2 operator+(const Vector2& other) const
	{
		return (Vector2(x + other.x, y + other.y));
	}

	Vector2 operator*(const float s)
	{
		return (Vector2(this->x * s, this->y * s));
	}

	Vector2 operator*=(const float s)
	{
		x *= s;
		y *= s;
		return (*this);
	}

	// To use Hash Table w/ vec2
	struct Vec2Hash
	{
		std::size_t operator()(const Vector2& v) const noexcept {
			std::size_t h1 = std::hash<float>{}(v.x);
			std::size_t h2 = std::hash<float>{}(v.y);
			return h1 ^ (h2 << 1);
		}
	};

	struct Vec2Eq
	{
		bool operator()(const Vector2& a, const Vector2& b) const noexcept {
			return a.x == b.x && a.y == b.y;
		}
	};

};

inline std::ostream& operator<<(std::ostream& out_stream, const Vector2& vec)
{
	out_stream << "[" << vec.x << " ," << vec.y << "]";
	return (out_stream);
};

inline Vector2 operator*=(const float s, Vector2& v)
{
	return (Vector2(
		v.x * s,
		v.y * s
	));
}

#endif