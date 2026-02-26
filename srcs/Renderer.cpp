#include "Renderer.hpp"
#include "GLMesh.hpp"
#include "Matrix4.hpp"
#include "Math.hpp"
#include "Camera.hpp"

Renderer::Renderer() : _shader("texture.vs", "texture.fs") {};

Renderer::~Renderer() {};

void Renderer::beginFrame()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(GLMesh& mesh, Camera& camera)
{
    _shader.use();

    // Projection
    mat4 projection = mat4::perspective(math::radians(camera.Zoom), camera.aspectRatio, 0.1f, 100.0f);
    _shader.setMat4("projection", projection);

    // View
    mat4 view = camera.GetViewMatrix();
    _shader.setMat4("view", view);

    // Model
    mat4 model = mat4::identity();
    _shader.setMat4("model", model);

    mesh.draw();
}
