#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"

class GLMesh;
class Camera;

class Renderer
{
	public:
		Renderer();
		~Renderer();

		void beginFrame();
		void draw(GLMesh& mesh, Camera& camera);
		void test();
	private:
		Shader	_light;
		Shader	_texture;
};

#endif