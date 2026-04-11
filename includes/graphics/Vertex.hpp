#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <iostream>
#include "Types.hpp"

struct Vertex
{
	Vector3 position;
	Vector3 normale;
	Vector2 texture;
	Vector3 color;

	Vertex() {};
	Vertex(const Vector3& pos, const Vector3& norm, const Vector2& text, const Vector3& color) : position(pos), normale(norm), texture(text), color(color) {};

	void print()
	{
		std::cout << "v" << position << ", vn" << normale << ", vt" << texture << ", color" << color << std::endl;
	};

	bool operator==(const Vertex& other) const
	{
		return (position == other.position && normale == other.normale && texture == other.texture);
	}
};

#endif