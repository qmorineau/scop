#include "Mesh.hpp"
#include "MeshBuilder.hpp"

void Mesh::addVertex(Vertex& v)
{
	int id = findDuplicateVertex(v);
	if (id != -1)
		_indices.push_back(id);
	else
	{
		_indices.push_back(_vertices.size());
		_vertices.push_back(v);
	}
};

int Mesh::findDuplicateVertex(Vertex& v)
{
	for (unsigned int i = 0; i < _vertices.size(); i++)
	{
		if (v == _vertices[i])
			return (i);
	}
	return (-1);
}