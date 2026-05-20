#include "Mesh.hpp"
#include "MeshBuilder.hpp"

void Mesh::addVertex(Vertex& v, Material* material)
{
	if (!subMeshes.size() || material != subMeshes[subMeshes.size() - 1].material)
	{
		SubMesh sm(material);
		subMeshes.push_back(sm);
	}
	SubMesh& submesh = subMeshes[subMeshes.size() - 1];
	submesh.indices.push_back(vertices.size());
	vertices.push_back(v);
};
