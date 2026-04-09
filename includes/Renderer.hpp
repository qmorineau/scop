#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"
#include "GLMesh.hpp"

struct Mesh;
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
		Renderer(Mesh&, std::string);
		~Renderer();

		void beginFrame();
		void setMode(RenderMode mode);
		void draw(Camera& camera, vec3& angle, std::vector<Light*> lights);
		void toggleWireframe() {_wireframe = !_wireframe;}
	private:
		Shader		_phong;
		RenderMode	_mode;
		bool		_wireframe = false;
		GLMesh		_glMesh;

		void configureMode();
};

#endif