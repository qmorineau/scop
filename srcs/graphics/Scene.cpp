#include "Scene.hpp"
#include "Window.hpp"

Scene::Scene(Mesh& mesh, std::string path) :
	_camera(SCR_WIDTH, SCR_HEIGHT),
	_glMesh(mesh, path)
{
	_isRotAxes = vec3(0., 1., 0.);
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
	if (_isRotAxes.x)
		_rotAngle.x += _rotations.x * _rotationSpeed * deltaTime;
	if (_isRotAxes.y)
		_rotAngle.y += _rotations.y * _rotationSpeed * deltaTime;
	if (_isRotAxes.z)
		_rotAngle.z += _rotations.z * _rotationSpeed * deltaTime;
}

void Scene::applyBlending(float deltaTime)
{
	if (_blend > 0.f && _blend < 1.f)
		_blend += _blending * _blendingSpeed * deltaTime;
}