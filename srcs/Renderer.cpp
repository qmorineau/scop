#include "Renderer.hpp"
#include "GLMesh.hpp"
#include "Matrix4.hpp"
#include "Math.hpp"
#include "Camera.hpp"
#include "Light.hpp"

Renderer::Renderer(Mesh& mesh, std::string path) :
	_shader("assets/shaders/shader.vs", "assets/shaders/shader.fs"),
	_mode(RenderMode::Phong),
	_glMesh(mesh, path)
{	
};

Renderer::~Renderer() {};

void Renderer::beginFrame()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(Camera& camera, vec3& angle, std::vector<Light*> lights, float blend)
{
	// Wireframe
	glPolygonMode(GL_FRONT_AND_BACK, _wireframe ? GL_LINE : GL_FILL);

	// Use shader
    _shader.use();

	// Camera
    mat4 projection = mat4::perspective(math::radians(camera.getZoom()), camera.getAspectRatio(), 0.001f, 100.0f);
    _shader.setMat4("projection", projection);
    _shader.setMat4("view", camera.getViewMatrix());
	_shader.setVec3("viewPos", vec3(camera.getPosition()));
	_shader.setFloat("u_textureBlend", blend);

	// Model
	mat4 model = mat4::rotateX(angle.x)
		.mul_mat(mat4::rotateY(angle.y))
		.mul_mat(mat4::rotateZ(angle.z));
    _shader.setMat4("model", model);

	// Configure Rendering Mode
	configureMode();

	if (true) // to change
	{
		_shader.setInt("lightCount", static_cast<int>(lights.size()));
		for (size_t i = 0; i < lights.size(); i++)
		{
			_shader.setVec3("lights[" + std::to_string(i) + "].position", lights[i]->getPosition());
			_shader.setVec3("lights[" + std::to_string(i) + "].color", lights[i]->getColor());
			_shader.setFloat("lights[" + std::to_string(i) + "].intensity", lights[i]->getIntensity());
			_shader.setBool("lights[" + std::to_string(i) + "].enabled", true);
		}
	}
	_glMesh.draw(_shader);
}

void Renderer::setMode(RenderMode mode)
{
	_mode = mode;
}

void Renderer::configureMode()
{
	 switch (_mode)
    {
        case RenderMode::Phong:
            _shader.setBool("u_useLighting", true);
            _shader.setBool("u_useTexture", false);
            _shader.setBool("u_overrideColor", false);
            break;

        case RenderMode::Face:
            _shader.setBool("u_useLighting", false);
            _shader.setBool("u_useTexture", false);
            _shader.setBool("u_overrideColor", true);
            break;

        case RenderMode::Material:
            _shader.setBool("u_useLighting", true);
            _shader.setBool("u_useTexture", true);
            _shader.setBool("u_overrideColor", false);
            break;
    }
};
