#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>
#include <algorithm>

#include <glad/glad.h>
#include <GLFW/glfw3.h> 

#define SCR_WIDTH 1920
#define SCR_HEIGHT 1080

// #define SCR_WIDTH 3840
// #define SCR_HEIGHT 2160

class Application;

class Window
{
	public:
		// Exception
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
		// Constructor / Destructor
		Window(Application*);
		~Window();
		// Method
		void 		manageTitle(Application& app);
		static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
		void 		closeWindow();
		// getter
		GLFWwindow* getWindow() {return _window;};
	private:
		GLFWwindow* _window;
		std::string _title;

};

#endif