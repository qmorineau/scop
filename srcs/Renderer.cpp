#include "Renderer.hpp"
#include "Types.hpp"
#include "GLMesh.hpp"

Renderer::Renderer() : _shader("texture.vs", "texture.fs") {};

Renderer::~Renderer() {};

void Renderer::beginFrame()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(GLMesh& mesh, Camera& camera)
{
	(void) camera; // next

	_shader.use();
	mesh.draw();
};
