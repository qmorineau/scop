#include "scop.hpp"
#include "Object.hpp"
#include "Vertex.hpp"
#include "Face.hpp"

Object::Object() {}

Object::Object(const Object& copy) {*this = copy;}

Object::Object(const std::string& file) : _filePath(file) {}

Object& Object::operator=(const Object& other)
{
	if (this != &other)
	{
		_filePath = other._filePath;
		_vertices = other._vertices;
		_faces = other._faces;
	}
	return (*this);
}

Object::~Object() 
{

}

void Object::parseVertex(std::istringstream& iss)
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1.0;

	iss >> x;
	iss >> y;
	iss >> z;
	iss >> w;

	Vertex v(x, y, z, w);
	_vertices.push_back(v);
}

void Object::parse()
{
	std::ifstream inFile(_filePath);

	if (!inFile.is_open())
	{
		// throw error
	}
	std::string line;
	std::string token;
	while (std::getline(inFile, line))
	{
		std::istringstream iss(line);
		if (iss >> token)
		{
			if (token.c_str()[0] == '#')
				continue;
			else if (!std::strcmp("v", token.c_str()))
				parseVertex(iss);
			else if (!std::strcmp("vt", token.c_str()))
				continue;
			else if (!std::strcmp("vn", token.c_str()))
				continue;
			else if (!std::strcmp("vp", token.c_str()))
				continue;
			else if (!std::strcmp("f", token.c_str()))
				continue;
			else
				continue; // wrong token, error ?
		}
	}
}

void Object::test()
{
	for	(unsigned int i = 0; i < _vertices.size(); i++)
	{
		std::cout << "ID " << i + 1 << " = ";
		_vertices[i].print();
	}
}