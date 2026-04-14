#ifndef MESHGPU_HPP
#define MESHGPU_HPP

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>

#include "Mesh.hpp"
#include "Types.hpp"

class Shader;

class MeshGPU
{
	public:
		MeshGPU(Mesh&, std::string);
		~MeshGPU();

		void draw(Shader&) const;

	private:
		GLuint		_vao = 0;
		GLuint		_vbo = 0;
		Mesh&	_mesh;

		void upload();
		void linkTexture(std::string&);
};

#endif