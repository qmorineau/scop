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
			ctx.mouse[button] = true;
		else if (action == GLFW_RELEASE)
		{
			ctx.mouse[button] = false;
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
		ctx.mouseMoved = true;
		ctx.mousePos.x = xposIn;
		ctx.mousePos.y = yposIn;
		ctx.ndc = vec2(
			(2.0f * xposIn) / SCR_WIDTH - 1.0f,
			1.0f - (2.0f * yposIn) / SCR_HEIGHT
		);
	}
}

void InputManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (app)
	{
		InputContext& ctx = app->inputContext();
		ctx.mouseScrolled = true;
		ctx.mouseOffset.x = xoffset;
		ctx.mouseOffset.y = yoffset;
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
		ctx.keys[key] = true;
    else if (action == GLFW_RELEASE)
	{
		ctx.keys[key] = false;
		app->inputHandler().handleKeysCallback(app, key);
	}
}
