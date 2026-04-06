#include "Renderer.hpp"
#include "GLMesh.hpp"
#include "Matrix4.hpp"
#include "Math.hpp"
#include "Camera.hpp"
#include "Light.hpp"

#include "stb_images.h"

Renderer::Renderer(std::vector<Mesh>& meshes) :
	_phong("assets/shaders/phong.vs", "assets/shaders/phong.fs"),
	_mode(RenderMode::Phong)
{	
	for (auto& mesh : meshes)
	{
		GLMesh glMesh(mesh);
		_glMeshes.push_back(glMesh);
	}
	test();
};

Renderer::~Renderer() {};

void Renderer::beginFrame()
{
	// glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // default opengl color
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(Camera& camera, vec3& angle, std::vector<Light*> lights)
{
	// Wireframe
	glPolygonMode(GL_FRONT_AND_BACK, _wireframe ? GL_LINE : GL_FILL);

	// Use shader
    _phong.use();

	// Camera
    mat4 projection = mat4::perspective(math::radians(camera.getZoom()), camera.getAspectRatio(), 0.1f, 100.0f);
    _phong.setMat4("projection", projection);
    _phong.setMat4("view", camera.GetViewMatrix());
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
		_phong.setInt("lightCount", lights.size());
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
            _phong.setVec3("u_overrideColorValue", vec3(1,0,0)); // exemple
            break;

        case RenderMode::Material:
            _phong.setBool("u_useLighting", true);
            _phong.setBool("u_useTexture", true);
            _phong.setBool("u_overrideColor", false);
            break;
    }
};

void Renderer::test()
{
	// load and create a texture 
    // -------------------------
    unsigned int texture1, texture2;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); 		
     // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load("assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed load image: " << stbi_failure_reason() << std::endl;
    }
    stbi_image_free(data);
    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed load image: " << stbi_failure_reason() << std::endl;
    }
    stbi_image_free(data);
	
	// _texture.use();

	// glUniform1i(glGetUniformLocation(_texture.ID, "texture1"), 0);
	// _texture.setInt("texture2", 1);
}