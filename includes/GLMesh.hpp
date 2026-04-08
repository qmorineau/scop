#ifndef GLMESH_HPP
#define GLMESH_HPP

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Mesh.hpp"
#include "Types.hpp"

class Shader;

class GLMesh
{
	public:
		GLMesh(Mesh&);
		~GLMesh();

		void draw(Shader&) const;

	private:
		GLuint		_vao = 0;
		GLuint		_vbo = 0;
		// GLuint		_ebo = 0;
		// GLsizei		_indexCount = 0;
		Mesh&	_mesh;

		void upload();
};

#endif