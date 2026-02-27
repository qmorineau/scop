#ifndef GLMESH_HPP
#define GLMESH_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Types.hpp"

class ObjMeshData;

class GLMesh
{
	public:
		GLMesh();
		~GLMesh();

		void upload(const ObjMeshData& mesh);
		void draw() const;

	private:
		GLuint _vao = 0;
		GLuint _vbo = 0;
		GLuint _ebo = 0;
		GLsizei _indexCount = 0;
};

#endif