#include "Scene.hpp"
#include "Window.hpp"

Scene::Scene(Mesh& mesh, std::string path) :
	_camera(SCR_WIDTH, SCR_HEIGHT),
	_model(mesh, path)
{}

Scene::~Scene() {}

void Scene::update(float deltaTime)
{
	_lights.update(deltaTime);
	_model.update(deltaTime);
};

void Scene::toggleLightEditor()
{
	_isLightEditor = !_isLightEditor;
	_lights.setColor(LightManager::ActiveColor::None);
};