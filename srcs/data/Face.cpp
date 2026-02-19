#include "Face.hpp"

Face::Face() {};

Face::Face(const Face& copy) {*this = copy;};

Face& Face::operator=(const Face&)
{
	return (*this);
};

Face::~Face() {};