#include "Mesh.hpp"
#include "Vertex.hpp"

Mesh::Mesh() {}

Mesh::Mesh(const Mesh& copy) {*this = copy;}

Mesh::Mesh(const std::string& file) : _filePath(file) {}

Mesh& Mesh::operator=(const Mesh& other)
{
	if (this != &other)
	{
		_filePath = other._filePath;
		_vertices = other._vertices;
	}
	return (*this);
}

Mesh::~Mesh() 
{

}

void Mesh::parsePosition(std::istringstream& iss)
{
	float x = 0;
	float y = 0;
	float z = 0;

	iss >> x;
	iss >> y;
	iss >> z;

	Vector3 v(x, y, z);
	_positions.push_back(v);
}

void Mesh::parseVertexNormal(std::istringstream& iss)
{
	float x = 0;
	float y = 0;
	float z = 0;

	iss >> x;
	iss >> y;
	iss >> z;

	Vector3 v(x, y, z);
	_normals.push_back(v);
}

void Mesh::parseTexture(std::istringstream& iss)
{
	float x = 0;
	float y = 0;

	iss >> x;
	iss >> y;

	Vector2 v(x, y);
	_textures.push_back(v);
}

int Mesh::findDuplicateVertex(Vertex& v)
{
	for (unsigned int i = 0; i < _vertices.size(); i++)
	{
		if (v == _vertices[i])
			return (i);
	}
	return (0);
}

void Mesh::parseVertex(std::string indices)
{
	std::string position = indices.substr(0, indices.find('/'));
	indices.erase(0, indices.find('/') + 1);
	std::string texture = indices.substr(0, indices.find('/'));
	indices.erase(0, indices.find('/') + 1);

	int v = atoi(position.c_str());
	int vt = atoi(texture.c_str());
	int vn = atoi(indices.c_str());
	
	Vertex vertex(_positions[v - 1], _normals[vn - 1], _textures[vt - 1]);
	int id = findDuplicateVertex(vertex);

	if (id)
		_indices.push_back(id);
	else
	{
		_indices.push_back(_vertices.size());
		_vertices.push_back(vertex);
	}
}

void Mesh::createFace(std::istringstream& iss)
{
	std::string word;

	for (int i = 0; i < 3; i++)
	{
		iss >> word;
		parseVertex(word);
	}
}

void Mesh::parse()
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
				parsePosition(iss);
			else if (!std::strcmp("vt", token.c_str()))
				parseTexture(iss);
			else if (!std::strcmp("vn", token.c_str()))
				parseVertexNormal(iss);
			else if (!std::strcmp("vp", token.c_str()))
				continue;
			else if (!std::strcmp("f", token.c_str()))
				createFace(iss);
			else
				continue; // wrong token, error ?
		}
	}
}

void Mesh::print()
{
	for	(unsigned int i = 0; i < _indices.size(); i++)
	{
		std::cout << "ID " << i + 1 << " = index " << _indices[i] << " = ";
		_vertices[_indices[i]].print();
	}
}