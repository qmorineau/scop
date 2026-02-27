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
		~Application();
		void run();

		static void processInput(GLFWwindow *window, Camera &camera, const float deltaTime);
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	private:
		Camera		_camera;
		Renderer*	_renderer;
		ObjMeshData	_mesh;
		GLFWwindow* _window;

		float		_deltaTime = 0.0f;
		float		_lastFrame = 0.0f;

		void initWindow();
		void renderLoop();
};

#endif