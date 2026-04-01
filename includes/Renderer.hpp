#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"

class GLMesh;
class Camera;
class Light;

enum class RenderMode
{
	Phong,
	Texture,
	Face
};

class Renderer
{
	public:
		Renderer();
		~Renderer();

		void beginFrame();
		void setMode(RenderMode mode);
		void draw(GLMesh& mesh, Camera& camera, std::vector<Light*> lights);
		void toggleWireframe() {_wireframe = !_wireframe;}
		void test();
	private:
		Shader		_light;
		Shader		_texture;
		RenderMode	_mode;
		bool		_wireframe;
};

#endif