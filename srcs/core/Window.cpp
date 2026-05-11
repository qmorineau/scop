#include "Window.hpp"
#include "Application.hpp"
#include "LightManager.hpp"
#include "InputManager.hpp"

Window::Window(Application* app)
{
	// glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

    // glfw window creation
    // --------------------
    _window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Scop", NULL, NULL);
    if (_window == NULL)
		throw GLWF_WindowError();

	glfwMakeContextCurrent(_window);
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(_window, app);

    glfwMakeContextCurrent(_window);
    glfwSetFramebufferSizeCallback(_window, Window::framebufferSizeCallback);
	glfwSetCursorPosCallback(_window, InputManager::mouseCallback);
    glfwSetScrollCallback(_window, InputManager::scrollCallback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw GLAD_Error();

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Window::~Window()
{
	if (_window)
	{
		glfwDestroyWindow(_window);
		_window = nullptr;
	}
	glfwTerminate();
}

void Window::manageTitle(Application& app)
{
	const LightManager& lights = app.lights();
	if (app.isLightEditor())
	{
		switch (lights.getColor())
		{
			case LightManager::ActiveColor::Red:
				_title = "Scop [Edit Light Mode][RED] ";
				break;
			case LightManager::ActiveColor::Green:
				_title = "Scop [Edit Light Mode][GREEN] ";
				break;
			case LightManager::ActiveColor::Blue:
				_title = "Scop [Edit Light Mode][BLUE] ";
				break;
			default:
				_title = "Scop [Edit Light Mode] ";
				break;
		}
	}
	else
		_title = "Scop ";
	float fps = 1.f / std::max(app.getDelta(), 0.0001f);
	_title.append("[" + std::to_string(static_cast<int>(fps)) + " fps]");
	glfwSetWindowTitle(_window, _title.c_str());
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	(void) window;
    glViewport(0, 0, width, height);
}

void Window::closeWindow()
{
	glfwSetWindowShouldClose(_window, true);
};