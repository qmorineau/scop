#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <iostream>

class Vertex
{
	public:
		Vertex();
		Vertex(const Vertex&);
		Vertex(const float x, const float y, const float z, const float w);
		Vertex& operator=(const Vertex&);
		~Vertex();

		void print();

	private:
		float _posx;
		float _posy;
		float _posz;
		float _w;
};

#endif