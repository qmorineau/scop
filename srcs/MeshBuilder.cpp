#include "MeshBuilder.hpp"

MeshBuilder::MeshBuilder(const std::vector<vec3>& p, const std::vector<vec3>& n, const std::vector<vec2>& uv, std::string name) :
	_positions(p),
	_normals(n),
	_uvs(uv),
	_mesh(name) {};

Mesh MeshBuilder::build()
{
	if (_faces.empty())
		throw EmptyMesh("");
	convertToGpuData();
	return _mesh;
}

void MeshBuilder::addTriangle(VertexIndex a, VertexIndex b, VertexIndex c, vec3 color)
{
	VertexIndex index[] = {a, b, c};
	Vertex vertex[3];
	for (int i = 0; i < 3; i++)
	{
		vertex[i] = Vertex(_positions[index[i].vertex], _normals[index[i].normal], _uvs[index[i].textCoord], color);
		_mesh.addVertex(vertex[i]);
	}
}

void MeshBuilder::convertToGpuData()
{
	for (auto it : _faces)
	{
		size_t i = 0;
		for (auto face : it.second)
		{
			std::vector<VertexIndex> &v = face.vertices;
			for (unsigned int j = 1; j < v.size() - 1; j++)
				addTriangle(v[0], v[j], v[j + 1], faceColors[i % 4]); // choose good mesh
			i++;
		}
	}
}

void MeshBuilder::addFace(Face& f, Material& m)
{
	_faces[m._name].push_back(f);
}