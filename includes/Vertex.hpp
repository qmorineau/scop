#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <iostream>
#include "Types.hpp"

struct Vertex
{
	Vector3 position;
	Vector3 normale;
	Vector2 texture;

	Vertex(Vector3& pos, Vector3& norm, Vector2& text) : position(pos), normale(norm), texture(text) {};

	void print()
	{
		std::cout << "v" << position << ", vn" << normale << ", vt" << texture << std::endl;
	};

	bool operator==(const Vertex& other) const
	{
		return (position == other.position && normale == other.normale && texture == other.texture);
	}
};

#endif