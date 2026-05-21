#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class InputManager
{
	public:
		InputManager() {};
		~InputManager() {};

		// Mouse
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void mouseCallback(GLFWwindow* window, double xposIn, double yposIn);
		static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		// Keyboard
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif
