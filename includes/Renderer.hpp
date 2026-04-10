#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"
#include "GLMesh.hpp"

struct Mesh;
class Camera;
class Light;
struct Material;

enum class RenderMode
{
	Phong,
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
		void draw(Camera& camera, vec3& angle, std::vector<Light*> lights, float blend);
		void toggleWireframe() {_wireframe = !_wireframe;}
	private:
		Shader		_shader;
		RenderMode	_mode;
		bool		_wireframe = false;
		GLMesh		_glMesh;
		
		void configureMode();
};

#endif