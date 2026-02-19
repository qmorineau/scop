#include "Vertex.hpp"

Vertex::Vertex(): _posx(0), _posy(0), _posz(0), _w(1.0f) {}

Vertex::Vertex(const Vertex& copy) {*this = copy;}

Vertex& Vertex::operator=(const Vertex& other)
{
	if (this != &other)
	{
		this->_posx = other._posx;
		this->_posy = other._posy;
		this->_posz = other._posz;
		this->_w = other._w;
	}
	return (*this);
}

Vertex::~Vertex() {};

Vertex::Vertex(const float x, const float y, const float z, const float w) : _posx(x), _posy(y), _posz(z), _w(w)
{

}

void Vertex::print()
{
	std::cout << "V = (" << _posx << ", " << _posy << ", " << _posz << ", [" << _w << "])\n";
}