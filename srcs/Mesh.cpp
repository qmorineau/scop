#include "Mesh.hpp"
#include "MeshBuilder.hpp"

// int Mesh::findDuplicateVertex(Vertex& v)
// {
// 	for (unsigned int i = 0; i < _vertices.size(); i++)
// 	{
// 		if (v == _vertices[i])
// 			return (i);
// 	}
// 	return (-1);
// }

// void Mesh::addTriangle(Vertex& a, Vertex& b, Vertex& c, vec3 color)
// {
// 	Vertex vertex[] = {a, b, c};
// 	for (int i = 0; i < 3; i++)
// 	{
// 		int id = findDuplicateVertex(vertex[i]);
// 		if (id != -1)
// 			_indices.push_back(id);
// 		else
// 		{
// 			_indices.push_back(_vertices.size());
// 			_vertices.push_back(vertex[i]);
// 		}
// 	}
// }
