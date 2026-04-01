#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <iostream>
#include <string>
#include <exception>
#include <glad/glad.h>
#include <GLFW/glfw3.h> 

#include "Camera.hpp"
#include "Renderer.hpp"
#include "ObjMeshData.hpp"
#include "GLMesh.hpp"
#include "Light.hpp"

#define SCR_WIDTH 1920
#define SCR_HEIGHT 1080

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
		~Application();
		void run();

		// static void processInput(GLFWwindow *window, Camera &camera, const float deltaTime);
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

		void toggleEditLight() {_editLight = !_editLight;};
		void setWindowTitle(std::string s) {_windowTitle = s;};

		bool _keys[1024] = {false};
		// put in private or in another class
		bool				_red = false;
		bool				_green = false;
		bool				_blue = false;
		bool				_editLight = false;
	private:
		Camera				_camera;
		Renderer*			_renderer;
		ObjMeshData			_mesh;
		GLFWwindow* 		_window;
		std::vector<Light*>	_lights;
		
		std::string			_windowTitle;
		float				_deltaTime = 0.0f;
		float				_lastFrame = 0.0f;

		void initWindow();
		void renderLoop();
};

#endif