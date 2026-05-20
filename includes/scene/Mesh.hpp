#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <string>
#include <cstdint>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Types.hpp"
#include "Material.hpp"

struct Mesh
{
	struct SubMesh
	{
		SubMesh(Material* m = nullptr) : material(m) {};

		GLuint					ebo = 0;
		std::vector<uint32_t>	indices;
		Material*			material;
	};
	Mesh() {};

	std::vector<Vertex>		vertices;
	std::vector<SubMesh>	subMeshes;
	
	void addVertex(Vertex& v, Material* material);
};

#endif