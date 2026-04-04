#include "MeshBuilder.hpp"

MeshBuilder::MeshBuilder(const std::vector<vec3>& p, const std::vector<vec3>& n, const std::vector<vec2>& uv) :
	_positions(p), _normals(n), _uvs(uv) {};

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

void MeshBuilder::build()
{

}

void MeshBuilder::addMesh(std::string name)
{
	Mesh newMesh(name);
	_meshes.push_back(newMesh);
}

void MeshBuilder::addFace(Face& f, Material& m)
{
	_faces[m._name].push_back(f);
}