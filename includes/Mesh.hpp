#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <string>

#include "Material.hpp"
#include "Types.hpp"

struct Mesh
{
	struct SubMesh
	{
		SubMesh(const Material* m) : material(m) {};

		GLuint					ebo = 0;
		std::vector<uint32_t>	indices;
		const Material*			material;
	};
	Mesh() {};

	std::vector<Vertex>		vertices;
	std::vector<SubMesh>	subMeshes;
	
	void addVertex(Vertex& v, const Material* material);
};

#endif