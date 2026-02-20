#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <iostream>
#include "Types.hpp"

struct Vertex
{
	Vector4 position;
	Vector3 normale;

	Vertex(float x, float y, float z) : position(x, y, z) {};
	Vertex(float x, float y, float z, float w) : position(x, y, z, w) {};

	void print()
	{
		std::cout << "position" << position << ", normale" << normale << std::endl;
	};

};

#endif