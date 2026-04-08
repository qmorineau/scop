#include "Renderer.hpp"
#include "GLMesh.hpp"
#include "Matrix4.hpp"
#include "Math.hpp"
#include "Camera.hpp"
#include "Light.hpp"

int idx;

Renderer::Renderer(Mesh& mesh) :
	_phong("assets/shaders/phong.vs", "assets/shaders/phong.fs"),
	_mode(RenderMode::Texture)
{	
	GLMesh glMesh(mesh);
	_glMeshes.push_back(glMesh);
	TextureLoader load;
	idx = load.loadTexture("assets/textures/awesomeface.png");
};

Renderer::~Renderer() {};

void Renderer::beginFrame()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(Camera& camera, vec3& angle, std::vector<Light*> lights)
{
	// Wireframe
	glPolygonMode(GL_FRONT_AND_BACK, _wireframe ? GL_LINE : GL_FILL);

	// Use shader
    _phong.use();

	// texture
	if (_mode == RenderMode::Texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, idx);
		_phong.linkTexture(0);
	}

	// Camera
    mat4 projection = mat4::perspective(math::radians(camera.getZoom()), camera.getAspectRatio(), 0.1f, 100.0f);
    _phong.setMat4("projection", projection);
    _phong.setMat4("view", camera.getViewMatrix());
	_phong.setVec3("viewPos", vec3(camera.getPosition()));

	// Model
	mat4 model = mat4::rotateX(angle.x)
		.mul_mat(mat4::rotateY(angle.y))
		.mul_mat(mat4::rotateZ(angle.z));
    _phong.setMat4("model", model);

	// Configure Rendering Mode
	configureMode();

	if (true) // to change
	{
		_phong.setInt("lightCount", static_cast<int>(lights.size()));
		for (size_t i = 0; i < lights.size(); i++)
		{
			_phong.setVec3("lights[" + std::to_string(i) + "].position", lights[i]->getPosition());
			_phong.setVec3("lights[" + std::to_string(i) + "].color", lights[i]->getColor());
			_phong.setFloat("lights[" + std::to_string(i) + "].intensity", lights[i]->getIntensity());
			_phong.setBool("lights[" + std::to_string(i) + "].enabled", true);
		}
	}

	// Draw
	for (auto& mesh : _glMeshes)
		mesh.draw(_phong);
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
            _phong.setBool("u_useLighting", true);
            _phong.setBool("u_useTexture", false);
            _phong.setBool("u_overrideColor", false);
            break;

        case RenderMode::Texture:
            _phong.setBool("u_useLighting", false);
            _phong.setBool("u_useTexture", true);
            _phong.setBool("u_overrideColor", false);
            break;

        case RenderMode::Face:
            _phong.setBool("u_useLighting", false);
            _phong.setBool("u_useTexture", false);
            _phong.setBool("u_overrideColor", true);
            break;

        case RenderMode::Material:
            _phong.setBool("u_useLighting", true);
            _phong.setBool("u_useTexture", true);
            _phong.setBool("u_overrideColor", false);
            break;
    }
};
