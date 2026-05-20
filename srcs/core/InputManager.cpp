#include "InputManager.hpp"
#include "Application.hpp"
#include "Camera.hpp"

// Mouse
void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) 
{
	(void) mods;
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (app)
	{
		InputContext& ctx = app->inputContext();
		if (action == GLFW_PRESS)
			ctx.pressMouse(button);
		else if (action == GLFW_RELEASE)
		{
			ctx.releaseMouse(button);
			app->inputHandler().handleMouseCallback(app, button);
		}
	}
}

void InputManager::mouseCallback(GLFWwindow* window, double xposIn, double yposIn) 
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (app)
	{
		InputContext& ctx = app->inputContext();
		ctx.setIsMouseMoved(true);
		ctx.setMousePos(vec2(xposIn, yposIn));
	}
}

void InputManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (app)
	{
		InputContext& ctx = app->inputContext();
		ctx.setIsMouseScrolled(true);
		ctx.setMouseScrolled(vec2(xoffset, yoffset));
	}
}
// Keyboard
void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	(void) scancode /* physical position of a key, not keyboard dependent */;
	(void) mods; /* bitmask if mod is press, shift, ctrl, alt, super... */

    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app)
		return;

	InputContext& ctx = app->inputContext();
	if (action == GLFW_PRESS)
		ctx.pressKey(key);
    else if (action == GLFW_RELEASE)
	{
		ctx.releaseKey(key);
		app->inputHandler().handleKeysCallback(app, key);
	}
}
