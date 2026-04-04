#ifndef GLMESH_HPP
#define GLMESH_HPP

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Mesh.hpp"
#include "Types.hpp"

// class ObjMeshData;

class GLMesh
{
	public:
		GLMesh();
		~GLMesh();

		void upload(const Mesh&);
		void draw() const;

	private:
		GLuint _vao = 0;
		GLuint _vbo = 0;
		GLuint _ebo = 0;
		GLsizei _indexCount = 0;
};

#endif