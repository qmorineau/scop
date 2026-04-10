#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <iostream>
#include <string>
#include <exception>

#include <glad/glad.h>
#include <GLFW/glfw3.h> 

#include "KeyHandler.hpp"
#include "ObjParser.hpp"
#include "Camera.hpp"
#include "LightManager.hpp"
#include "Renderer.hpp"
#include "GLMesh.hpp"
#include "Light.hpp"

#define SCR_WIDTH 1920
#define SCR_HEIGHT 1080

// #define SCR_WIDTH 3840
// #define SCR_HEIGHT 2160

class Application
{
	// Exceptions
	class GLWF_WindowError : public std::exception
	{
		public:
			GLWF_WindowError() {};
			const char* what() const noexcept override {return "Failed to create GLFW window";}
	};
	class GLAD_Error : public std::exception
	{
		public:
			GLAD_Error() {};
			const char* what() const noexcept override {return "Failed to initialize GLAD";}
	};

	public:
		Application(char *file);
		void processInput();
		void lightEditor();
		~Application();
		void run();

		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void mouseCallback(GLFWwindow* window, double xposIn, double yposIn);
		static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		bool isLightEditing() {return _editLight;};
		void closeWindow() {glfwSetWindowShouldClose(_window, true);};
		void setColor(LightManager::ActiveColor c) {_lightManager.setColor(c);};
		void toggleEditLight()
		{
			_editLight = !_editLight;
			_lightManager.setColor(LightManager::ActiveColor::None);
		};
		void setWindowTitle(std::string s) {_windowTitle = s;};
		float	getDelta() {return _deltaTime;};
		Renderer*	getRenderer() {return _renderer;};

		LightManager& getLightManager() { return _lightManager;};
		void setKey(int key, bool state) {_keyHandler.setKey(key, state);};
		GLFWwindow*	getWindow() {return _window;};
		bool				_editLight = false;

		LightManager		_lightManager;
		Camera				_camera;
		vec3				_rotAngle;
		float				_blend = 0.f;
		float				_blending = -0.01f;
		KeyHandler			_keyHandler;
		Renderer*			_renderer;
		ObjParser			_parser;
		GLFWwindow* 		_window;
		vec3				_isRotAxes;
		vec3				_rotations;
	private:
		
		std::string			_windowTitle;
		float				_deltaTime = 0.0f;
		float				_lastFrame = 0.0f;

		void initWindow();
		void applyRotation();
		void renderLoop();
};

#endif