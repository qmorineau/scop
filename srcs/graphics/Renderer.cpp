#include "Renderer.hpp"
#include "MeshGPU.hpp"
#include "Matrix4.hpp"
#include "Math.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Light.hpp"

Renderer::Renderer() :
	_shader("assets/shaders/shader.vs", "assets/shaders/shader.fs"),
	_mode(RenderMode::Phong),
	_faceRender(FaceRendering::Full)
{};

Renderer::~Renderer() {};

void Renderer::beginFrame()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(Scene* scene)
{
	if (_faceRender == FaceRendering::VertexOnly)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	else if (_faceRender == FaceRendering::Wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Use shader
    _shader.use();

	// Camera
	Camera& camera = scene->camera();
    mat4 projection = mat4::perspective(math::radians(camera.getZoom()), camera.getAspectRatio(), 0.001f, 100.0f);
    _shader.setMat4("projection", projection);
    _shader.setMat4("view", camera.getViewMatrix());
	_shader.setVec3("viewPos", vec3(camera.getPosition()));
	_shader.setFloat("u_textureBlend", scene->getBlend());

	// Model
	const vec3 angle = scene->getRotAngle();
	mat4 model = mat4::rotateX(angle.x)
		.mul_mat(mat4::rotateY(angle.y))
		.mul_mat(mat4::rotateZ(angle.z));
    _shader.setMat4("model", model);

	// Configure Rendering Mode
	configureMode();

	const std::vector<Light*>& lights = scene->lights().getLights();
	_shader.setInt("lightCount", static_cast<int>(lights.size()));
	for (size_t i = 0; i < lights.size(); i++)
	{
		_shader.setVec3("lights[" + std::to_string(i) + "].position", lights[i]->getPosition());
		_shader.setVec3("lights[" + std::to_string(i) + "].color", lights[i]->getColor());
		_shader.setFloat("lights[" + std::to_string(i) + "].intensity", lights[i]->getIntensity());
		_shader.setBool("lights[" + std::to_string(i) + "].enabled", true);
	}
	const MeshGPU& mesh = scene->mesh();
	mesh.draw(_shader);
}

void Renderer::setMode(RenderMode mode)
{
	_mode = mode;
}

void Renderer::changeFaceRendering()
{
	switch (_faceRender)
	{
		case FaceRendering::VertexOnly:
			_faceRender = FaceRendering::Wireframe;
			break;
		case FaceRendering::Wireframe:
			_faceRender = FaceRendering::Full;
			break;
		case FaceRendering::Full:
			_faceRender = FaceRendering::VertexOnly;
			break;
	}
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
