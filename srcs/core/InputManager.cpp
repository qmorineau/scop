#include "InputManager.hpp"
#include "LightManager.hpp"
#include "Application.hpp"
#include "Light.hpp"
#include "Camera.hpp"

void InputManager::handleKeys(Application* app)
{
	Camera& camera = app->getCamera();
	if (_keys[GLFW_KEY_W])
		camera.processKeyboard(Camera::FORWARD, app->getDelta());
	if (_keys[GLFW_KEY_S])
		camera.processKeyboard(Camera::BACKWARD, app->getDelta());
	if (_keys[GLFW_KEY_A])
		camera.processKeyboard(Camera::LEFT, app->getDelta());
	if (_keys[GLFW_KEY_D])
		camera.processKeyboard(Camera::RIGHT, app->getDelta());
	if (app->isLightEditor())
	{
		LightManager& manager = app->lights();
		lightEditor(manager, app->getDelta());
	}
};

// Light Handlers
const std::unordered_map<int, InputManager::Handler> InputManager::lightHandlers =
{
	{GLFW_KEY_ESCAPE, &InputManager::closeWindow},
	{GLFW_KEY_R, &InputManager::editRed},
	{GLFW_KEY_G, &InputManager::editGreen},
	{GLFW_KEY_B, &InputManager::editBlue},
	{GLFW_KEY_M, &InputManager::changeMode},
	{GLFW_KEY_N, &InputManager::addLight},
	{GLFW_KEY_E, &InputManager::deleteLight},
	{GLFW_KEY_L, &InputManager::toggleLightMod},
	{GLFW_KEY_LEFT, &InputManager::prevLight},
	{GLFW_KEY_RIGHT, &InputManager::nextLight}
};

void InputManager::closeWindow(Application* app)			{app->closeWindow();}
void InputManager::addLight(Application* app)				{app->lights().add(app->getCamera().getPosition());}
void InputManager::deleteLight(Application* app)			{app->lights().remove();}
void InputManager::editRed(Application* app)				{app->lights().setColor(LightManager::ActiveColor::Red);}
void InputManager::editGreen(Application* app)			{app->lights().setColor(LightManager::ActiveColor::Green);}
void InputManager::editBlue(Application* app)				{app->lights().setColor(LightManager::ActiveColor::Blue);}
void InputManager::toggleLightMod(Application* app)		{app->toggleLightEditor();}
void InputManager::changeFaceRendering(Application* app)	{app->renderer().changeFaceRendering();}
void InputManager::changeMode(Application* app)			{app->getCamera().changeMode();}
void InputManager::nextLight(Application* app)
{
	Light* l = app->lights().next();
	if (l)
		app->getCamera().changePosition(l->getPosition());
}
void InputManager::prevLight(Application* app)
{
	Light* l = app->lights().prev();
	if (l)
		app->getCamera().changePosition(l->getPosition());
}

// Key Handlers

const std::unordered_map<int, InputManager::Handler> InputManager::handlers =
{
	{GLFW_KEY_ESCAPE, &InputManager::closeWindow},
	{GLFW_KEY_1, &InputManager::setPhong},
	{GLFW_KEY_2, &InputManager::setFace},
	{GLFW_KEY_3, &InputManager::setMaterial},
	{GLFW_KEY_P, &InputManager::changeFaceRendering},
	{GLFW_KEY_M, &InputManager::changeMode},
	{GLFW_KEY_T, &InputManager::applyTexture},
	{GLFW_KEY_X, &InputManager::rotateX},
	{GLFW_KEY_Y, &InputManager::rotateY},
	{GLFW_KEY_Z, &InputManager::rotateZ},
	{GLFW_KEY_L, &InputManager::toggleLightMod},
	{GLFW_KEY_R, &InputManager::resetCam}
};
void InputManager::setPhong(Application* app)		{app->renderer().setMode(RenderMode::Phong);}
void InputManager::setFace(Application* app)		{app->renderer().setMode(RenderMode::Face);}
void InputManager::setMaterial(Application* app)	{app->renderer().setMode(RenderMode::Material);}
void InputManager::applyTexture(Application* app)
{
	app->scene()->toggleTexture();
}
void InputManager::resetCam(Application* app)
{
	app->getCamera().resetPosition();
	app->scene()->resetRot();
}
void InputManager::rotateX(Application* app)
{
	app->scene()->rotateX();
}

void InputManager::rotateY(Application* app)
{
	app->scene()->rotateY();
}

void InputManager::rotateZ(Application* app)
{
	app->scene()->rotateZ();
}

// Callbacks

void InputManager::mouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (app)
	    app->getCamera().onMouseMove(xposIn, yposIn);
}

void InputManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (app)
		app->getCamera().onMouseScroll(xoffset, yoffset);
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	(void) scancode /* physical position of a key, not keyboard dependent */;
	(void) mods; /* bitmask if mod is press, shift, ctrl, alt, super... */

    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app)
		return;

	if (action == GLFW_PRESS) {app->setKey(key, true);}
    else if (action == GLFW_RELEASE) {app->setKey(key, false);}

	if (action == GLFW_RELEASE)
	{
		if (app->isLightEditor())
		{
			auto iterator = lightHandlers.find(key);
			if (iterator == lightHandlers.end())
				return;
			Handler h = iterator->second;
			(app->inputManager().*h)(app);
		}
		else
		{
			auto iterator = handlers.find(key);
			if (iterator == handlers.end())
				return;
			Handler h = iterator->second;
			(app->inputManager().*h)(app);
		}
	}
}

void InputManager::lightEditor(LightManager& manager, float deltaTime)
{
	if (_keys[GLFW_KEY_1])
		manager.changeColor(vec3(1, 1, 1));
	if (_keys[GLFW_KEY_2])
		manager.changeColor(vec3(0, 0, 0));
	if (_keys[GLFW_KEY_3])
		manager.changeColor(vec3(1, 0, 0));
	if (_keys[GLFW_KEY_4])
		manager.changeColor(vec3(0, 1, 0));
	if (_keys[GLFW_KEY_5])
		manager.changeColor(vec3(0, 0, 1));
	if (_keys[GLFW_KEY_6])
		manager.changeColor(vec3(1, 1, 0));
	if (_keys[GLFW_KEY_7])
		manager.changeColor(vec3(1, 0, 1));
	if (_keys[GLFW_KEY_8])
		manager.changeColor(vec3(0, 1, 1));
	if (_keys[GLFW_KEY_9])
	{
		switch (manager.getColor())
		{
			case LightManager::ActiveColor::Red:
				manager.colorDown(Color::Red, deltaTime);
				break;
			case LightManager::ActiveColor::Green:
				manager.colorDown(Color::Green, deltaTime);
				break;
			case LightManager::ActiveColor::Blue:
				manager.colorDown(Color::Blue, deltaTime);
				break;
			default:
				break;
		}
	}
	if (_keys[GLFW_KEY_0])
	{
		switch (manager.getColor())
		{
			case LightManager::ActiveColor::Red:
				manager.colorUp(Color::Red, deltaTime);
				break;
			case LightManager::ActiveColor::Green:
				manager.colorUp(Color::Green, deltaTime);
				break;
			case LightManager::ActiveColor::Blue:
				manager.colorUp(Color::Blue, deltaTime);
				break;
			default:
				break;
		}
	}
	if (_keys[GLFW_KEY_UP])
		manager.intensityUp(deltaTime);
	if (_keys[GLFW_KEY_DOWN])
		manager.intensityDown(deltaTime);
}
