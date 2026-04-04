#include "Mesh.hpp"
#include "MeshBuilder.hpp"

void Mesh::addVertex(Vertex& v, const Material* material)
{
	if (!subMeshes.size() || material != subMeshes[subMeshes.size() - 1].material)
	{
		SubMesh sm(material);
		subMeshes.push_back(sm);
	}
	SubMesh& submesh = subMeshes[subMeshes.size() - 1];
	int id = findDuplicateVertex(v);
	if (id != -1)
		submesh.indices.push_back(id);
	else
	{
		submesh.indices.push_back(vertices.size());
		vertices.push_back(v);
	}
};

int Mesh::findDuplicateVertex(Vertex& v)
{
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		if (v == vertices[i])
			return (i);
	}
	return (-1);
}