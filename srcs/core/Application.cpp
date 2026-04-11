#include "Application.hpp"

Application::Application(char *file) :
	_camera(SCR_WIDTH, SCR_HEIGHT),
	_renderer(nullptr),
	_parser(std::string(file)),
	_window(nullptr),
	_isRotAxes(0., 1., 0.),
	_rotations(vec3(0.01, 0.01, 0.01))
{}
		

Application::~Application()
{
	if (_renderer)
		delete _renderer;
	if (_window)
	{
		glfwDestroyWindow(_window);
		_window = nullptr;
	}
	glfwTerminate();
}

void Application::run()
{
	initWindow();
	_renderer = new Renderer(_parser.getMeshes(), _parser.getPath());
	renderLoop();
}

void Application::initWindow()
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
	glfwSetWindowUserPointer(_window, this);

    glfwMakeContextCurrent(_window);
    glfwSetFramebufferSizeCallback(_window, framebufferSizeCallback);
	glfwSetCursorPosCallback(_window, mouseCallback);
    glfwSetScrollCallback(_window, scrollCallback);

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

void Application::applyRotation()
{
	if (_isRotAxes.x)
		_rotAngle.x += _rotations.x * _rotationSpeed * _deltaTime;
	if (_isRotAxes.y)
		_rotAngle.y += _rotations.y * _rotationSpeed * _deltaTime;
	if (_isRotAxes.z)
		_rotAngle.z += _rotations.z * _rotationSpeed * _deltaTime;
}

void Application::manageWindowTitle()
{
	if (_editLight)
	{
		switch (_lightManager.getColor())
		{
			case LightManager::ActiveColor::Red:
				_windowTitle = "Scop [Edit Light Mode][RED] ";
				break;
			case LightManager::ActiveColor::Green:
				_windowTitle = "Scop [Edit Light Mode][GREEN] ";
				break;
			case LightManager::ActiveColor::Blue:
				_windowTitle = "Scop [Edit Light Mode][BLUE] ";
				break;
			default:
				_windowTitle = "Scop [Edit Light Mode] ";
				break;
		}
	}
	else
		_windowTitle = "Scop ";
	_windowTitle.append("[" + std::to_string(static_cast<int>(1 / _deltaTime)) + " fps]");
	glfwSetWindowTitle(_window, _windowTitle.c_str());
}

void Application::renderLoop()
{
	glfwSwapInterval(0); // disable vsync

	while (!glfwWindowShouldClose(_window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		_deltaTime = currentFrame - _lastFrame;
		_lastFrame = currentFrame;
		manageWindowTitle();
		glfwSetKeyCallback(_window, KeyHandler::keyCallback);
		_keyHandler.handleKeys(this);
		applyRotation();
		if (_blend > 0.f && _blend < 1.f)
			_blend += _blending * _blendingSpeed * _deltaTime;
		_renderer->beginFrame();
		_renderer->draw(_camera, _rotAngle, _lightManager.getLights(), _blend);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(_window);
        glfwPollEvents();
	}
}

// Callback for any resize window event
void Application::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	(void) window;
    glViewport(0, 0, width, height);
}

void Application::mouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (app)
	    app->_camera.onMouseMove(xposIn, yposIn);
}

// Callback for mouse scroll event
void Application::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (app)
		app->_camera.onMouseScroll(xoffset, yoffset);
}