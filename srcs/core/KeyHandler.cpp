#include "KeyHandler.hpp"
#include "LightManager.hpp"
#include "Application.hpp"
#include "Light.hpp"

void KeyHandler::handleKeys(Application* app)
{
	if (!app->isLightEditing())
	{
		if (_keys[GLFW_KEY_W])
			app->_camera.processKeyboard(Camera::FORWARD, app->getDelta());
		if (_keys[GLFW_KEY_S])
			app->_camera.processKeyboard(Camera::BACKWARD, app->getDelta());
		if (_keys[GLFW_KEY_A])
			app->_camera.processKeyboard(Camera::LEFT, app->getDelta());
		if (_keys[GLFW_KEY_D])
			app->_camera.processKeyboard(Camera::RIGHT, app->getDelta());
	}
	else
	{
		LightManager& manager = app->getLightManager();
		lightEditor(manager, app->getDelta());
	}
};

// Light Handlers
const std::unordered_map<int, KeyHandler::Handler> KeyHandler::lightHandlers =
{
	{GLFW_KEY_ESCAPE, &KeyHandler::closeWindow},
	{GLFW_KEY_R, &KeyHandler::editRed},
	{GLFW_KEY_G, &KeyHandler::editGreen},
	{GLFW_KEY_B, &KeyHandler::editBlue},
	{GLFW_KEY_N, &KeyHandler::addLight},
	{GLFW_KEY_E, &KeyHandler::deleteLight},
	{GLFW_KEY_L, &KeyHandler::toggleLightMod},
	{GLFW_KEY_A, &KeyHandler::prevLight},
	{GLFW_KEY_D, &KeyHandler::nextLight}
};

void KeyHandler::closeWindow(Application* app)			{app->closeWindow();}
void KeyHandler::addLight(Application* app)				{app->_lightManager.add(app->_camera.getPosition());}
void KeyHandler::deleteLight(Application* app)			{app->_lightManager.remove();}
void KeyHandler::editRed(Application* app)				{app->setColor(LightManager::ActiveColor::Red);}
void KeyHandler::editGreen(Application* app)			{app->setColor(LightManager::ActiveColor::Green);}
void KeyHandler::editBlue(Application* app)				{app->setColor(LightManager::ActiveColor::Blue);}
void KeyHandler::toggleLightMod(Application* app)		{app->toggleEditLight();}
void KeyHandler::changeFaceRendering(Application* app)	{app->getRenderer()->changeFaceRendering();}
void KeyHandler::changeMode(Application* app)			{app->_camera.changeMode();}
void KeyHandler::nextLight(Application* app)
{
	Light* l = app->_lightManager.next();
	if (l)
		app->_camera.changePosition(l->getPosition());
}
void KeyHandler::prevLight(Application* app)
{
	Light* l = app->_lightManager.prev();
	if (l)
		app->_camera.changePosition(l->getPosition());
}

// Key Handlers

const std::unordered_map<int, KeyHandler::Handler> KeyHandler::handlers =
{
	{GLFW_KEY_ESCAPE, &KeyHandler::closeWindow},
	{GLFW_KEY_1, &KeyHandler::setPhong},
	{GLFW_KEY_2, &KeyHandler::setFace},
	{GLFW_KEY_3, &KeyHandler::setMaterial},
	{GLFW_KEY_P, &KeyHandler::changeFaceRendering},
	{GLFW_KEY_M, &KeyHandler::changeMode},
	{GLFW_KEY_T, &KeyHandler::applyTexture},
	{GLFW_KEY_X, &KeyHandler::rotateX},
	{GLFW_KEY_Y, &KeyHandler::rotateY},
	{GLFW_KEY_Z, &KeyHandler::rotateZ},
	{GLFW_KEY_L, &KeyHandler::toggleLightMod},
	{GLFW_KEY_R, &KeyHandler::resetCam}
};
void KeyHandler::setPhong(Application* app)		{app->getRenderer()->setMode(RenderMode::Phong);}
void KeyHandler::setFace(Application* app)		{app->getRenderer()->setMode(RenderMode::Face);}
void KeyHandler::setMaterial(Application* app)	{app->getRenderer()->setMode(RenderMode::Material);}
void KeyHandler::applyTexture(Application* app)
{
	app->_blending = -app->_blending;
	app->_blend += app->_blending;
}
void KeyHandler::resetCam(Application* app)
{
	app->_camera.resetPosition();
	app->_rotAngle = vec3(0,0,0);
}
void KeyHandler::rotateX(Application* app)
{
	if (app->_isRotAxes.x)
		app->_isRotAxes.x = 0.f;
	else
	{
		app->_isRotAxes.x = 1.f;
		app->_rotations.x = -app->_rotations.x;
	}
}

void KeyHandler::rotateY(Application* app)
{
	if (app->_isRotAxes.y)
		app->_isRotAxes.y = 0.f;
	else
	{
		app->_isRotAxes.y = 1.f;
		app->_rotations.y = -app->_rotations.y;
	}
}

void KeyHandler::rotateZ(Application* app)
{
	if (app->_isRotAxes.z)
		app->_isRotAxes.z = 0.f;
	else
	{
		app->_isRotAxes.z = 1.f;
		app->_rotations.z = -app->_rotations.z;
	}
}

void KeyHandler::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
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
		if (app->isLightEditing())
		{
			auto iterator = lightHandlers.find(key);
			if (iterator == lightHandlers.end())
				return;
			Handler h = iterator->second;
			(app->_keyHandler.*h)(app);
		}
		else
		{
			auto iterator = handlers.find(key);
			if (iterator == handlers.end())
				return;
			Handler h = iterator->second;
			(app->_keyHandler.*h)(app);
		}
	}
}

void KeyHandler::lightEditor(LightManager& manager, float deltaTime)
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
	if (_keys[GLFW_KEY_W])
		manager.intensityUp(deltaTime);
	if (_keys[GLFW_KEY_S])
		manager.intensityDown(deltaTime);
}
