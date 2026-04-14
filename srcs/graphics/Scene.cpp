#include "Scene.hpp"
#include "Window.hpp"

Scene::Scene(Mesh& mesh, std::string path) :
	_camera(SCR_WIDTH, SCR_HEIGHT),
	_MeshGPU(mesh, path)
{
	_rotationsEnable = vec3(0., 1., 0.);
	_rotations = vec3(0.01, 0.01, 0.01);
	_blend = 0.f;
	_blending = -0.01f;
}

Scene::~Scene() {}

void Scene::update(float deltaTime)
{
	applyRotation(deltaTime);
	applyBlending(deltaTime);
};

void Scene::applyRotation(float deltaTime)
{
	if (_rotationsEnable.x)
		_rotAngle.x += _rotations.x * _rotationSpeed * deltaTime;
	if (_rotationsEnable.y)
		_rotAngle.y += _rotations.y * _rotationSpeed * deltaTime;
	if (_rotationsEnable.z)
		_rotAngle.z += _rotations.z * _rotationSpeed * deltaTime;
}

void Scene::rotateX()
{
	if (_rotationsEnable.x)
		_rotationsEnable.x = 0.f;
	else
	{
		_rotationsEnable.x = 1.f;
		_rotations.x = -_rotations.x;
	}
}

void Scene::rotateY()
{
	if (_rotationsEnable.y)
		_rotationsEnable.y = 0.f;
	else
	{
		_rotationsEnable.y = 1.f;
		_rotations.y = -_rotations.y;
	}
}

void Scene::rotateZ()
{
	if (_rotationsEnable.z)
		_rotationsEnable.z = 0.f;
	else
	{
		_rotationsEnable.z = 1.f;
		_rotations.z = -_rotations.z;
	}
}

void Scene::toggleTexture()
{
	_blending = -_blending;
	_blend += _blending;
};

void Scene::applyBlending(float deltaTime)
{
	if (_blend > 0.f && _blend < 1.f)
		_blend += _blending * _blendingSpeed * deltaTime;
	else
		_blend = _blend >= 1.f ? 1.f : 0.f;
}

void Scene::toggleLightEditor()
{
	_isLightEditor = !_isLightEditor;
	_lights.setColor(LightManager::ActiveColor::None);
};