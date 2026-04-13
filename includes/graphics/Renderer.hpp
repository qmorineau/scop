#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"

class Scene;

enum class RenderMode
{
	Phong,
	Face,
	Material
};

enum class FaceRendering
{
	VertexOnly,
	Wireframe,
	Full
};

class Renderer
{
	public:
		Renderer();
		~Renderer();

		void beginFrame();
		void setMode(RenderMode mode);
		void changeFaceRendering();
		void draw(Scene* scene);
	private:
		Shader			_shader;
		RenderMode		_mode;
		FaceRendering	_faceRender;
		
		void configureMode();
};

#endif