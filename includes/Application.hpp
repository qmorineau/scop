#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <iostream>
#include <string>
#include <exception>
#include <glad/glad.h>
#include <GLFW/glfw3.h> 

#include "Camera.hpp"
#include "Renderer.hpp"
#include "Mesh.hpp"
#include "GLMesh.hpp"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

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

	private:
		Camera		_camera;
		Renderer*	_renderer;
		Mesh		_mesh;
		GLFWwindow* _window;

		void initWindow();
		void renderLoop();
};

#endif