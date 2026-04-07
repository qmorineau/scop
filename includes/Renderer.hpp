#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"

class Mesh;
class GLMesh;
class Camera;
class Light;

enum class RenderMode
{
	Phong,
	Texture,
	Face,
	Material
};

class Renderer
{
	public:
		Renderer(Mesh&);
		~Renderer();

		void beginFrame();
		void setMode(RenderMode mode);
		void draw(Camera& camera, vec3& angle, std::vector<Light*> lights);
		void toggleWireframe() {_wireframe = !_wireframe;}
		void test();
	private:
		Shader				_phong;
		RenderMode			_mode;
		bool				_wireframe = false;
		std::vector<GLMesh>	_glMeshes;

		void configureMode();
};

#endif