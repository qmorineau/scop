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
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(_window, mouse_callback);
    glfwSetScrollCallback(_window, scroll_callback);

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
		_rotAngle.x += _rotations.x;
	if (_isRotAxes.y)
		_rotAngle.y += _rotations.y;
	if (_isRotAxes.z)
		_rotAngle.z += _rotations.z;
}

void Application::renderLoop()
{
	// glfwSwapInterval(0); // disable vsync

	while (!glfwWindowShouldClose(_window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		_deltaTime = currentFrame - _lastFrame;
		_lastFrame = currentFrame;
		if (_editLight)
			_windowTitle = "Scop [Edit Light Mode] ";
		else
			_windowTitle = "Scop ";
		_windowTitle.append("[" + std::to_string(static_cast<int>(1 / _deltaTime)) + " fps]");
		glfwSetWindowTitle(_window, _windowTitle.c_str());
		glfwSetKeyCallback(_window, Application::keyCallback);
		processInput();
		applyRotation();
		if (_blend > 0.f && _blend < 1.f)
			_blend += _blending;
		 // Rendering
		_renderer->beginFrame();
		_renderer->draw(_camera, _rotAngle, _lightManager.getLights(), _blend);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(_window);
        glfwPollEvents();
	}
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
// void Application::processInput(GLFWwindow *window, Camera &camera, const float deltaTime)

void Application::lightEditor()
{
	if (_keys[GLFW_KEY_1])
		_lightManager.changeColor(vec3(1, 1, 1));
	if (_keys[GLFW_KEY_2])
		_lightManager.changeColor(vec3(1, 0, 0));
	if (_keys[GLFW_KEY_3])
		_lightManager.changeColor(vec3(0, 1, 0));
	if (_keys[GLFW_KEY_4])
		_lightManager.changeColor(vec3(0, 0, 1));
	if (_keys[GLFW_KEY_5])
		_lightManager.changeColor(vec3(1, 1, 0));
	if (_keys[GLFW_KEY_6])
		_lightManager.changeColor(vec3(1, 0, 1));
	if (_keys[GLFW_KEY_7])
		_lightManager.changeColor(vec3(0, 1, 1));
	if (_keys[GLFW_KEY_W])
		_lightManager.intensityUp();
	if (_keys[GLFW_KEY_S])
		_lightManager.intensityDown();
	if (_keys[GLFW_KEY_0])
	{
		if (_red)
			_lightManager.colorUp(Color::Red);
		if (_green)
			_lightManager.colorUp(Color::Green);
		if (_blue)
			_lightManager.colorUp(Color::Blue);
	}
	if (_keys[GLFW_KEY_9])
	{
		if (_red)
			_lightManager.colorDown(Color::Red);
		if (_green)
			_lightManager.colorDown(Color::Green);
		if (_blue)
			_lightManager.colorDown(Color::Blue);
	}
}

void Application::processInput()
{
	if (!_editLight)
	{
		if (_keys[GLFW_KEY_W])
			_camera.processKeyboard(Camera::FORWARD, _deltaTime);
		if (_keys[GLFW_KEY_S])
			_camera.processKeyboard(Camera::BACKWARD, _deltaTime);
		if (_keys[GLFW_KEY_A])
			_camera.processKeyboard(Camera::LEFT, _deltaTime);
		if (_keys[GLFW_KEY_D])
			_camera.processKeyboard(Camera::RIGHT, _deltaTime);
	}
	else
		lightEditor();
};

void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	(void) scancode /* physical position of a key, not keyboard dependent */;
	(void) mods; /* bitmask if mod is press, shift, ctrl, alt, super... */

    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app)
		return;

	if (action == GLFW_PRESS) {app->_keys[key] = true;}
    else if (action == GLFW_RELEASE) {app->_keys[key] = false;}

	if (action == GLFW_RELEASE)
	{
		if (app->_editLight)
		{
			Light* l;
			switch(key)
			{
				case GLFW_KEY_ESCAPE:
					glfwSetWindowShouldClose(window, true);
					break;
				case GLFW_KEY_A:
					l = app->_lightManager.prev();
					if (l)
						app->_camera.changePosition(l->getPosition());
					break;
				case GLFW_KEY_D:
					l = app->_lightManager.next();
					if (l)
						app->_camera.changePosition(l->getPosition());
					break;
				case GLFW_KEY_R:
					app->_red = true;
					app->_green = false;
					app->_blue = false;
					break;
				case GLFW_KEY_G:
					app->_red = false;
					app->_green = true;
					app->_blue = false;
					break;
				case GLFW_KEY_B:
					app->_red = false;
					app->_green = false;
					app->_blue = true;
					break;
				case GLFW_KEY_N:
					app->_lightManager.add(app->_camera.getPosition());
					break;
				case GLFW_KEY_E:
					app->_lightManager.remove();
					break;
				case GLFW_KEY_L:
					app->toggleEditLight();
					break;
			}
		}
		else
		{
			switch (key)
			{
				case GLFW_KEY_ESCAPE:
					glfwSetWindowShouldClose(window, true);
					break;
				case GLFW_KEY_1:
					app->_renderer->setMode(RenderMode::Phong);
					break;
				case GLFW_KEY_2:
					app->_renderer->setMode(RenderMode::Face);
					break;
				case GLFW_KEY_3:
					app->_renderer->setMode(RenderMode::Material);
					break;
				case GLFW_KEY_P:
					app->_renderer->toggleWireframe();
					break;
				case GLFW_KEY_M:
					app->_camera.changeMode();
					break;
				case GLFW_KEY_T:
					app->_blending = -app->_blending;
					app->_blend += app->_blending;
					break;
				case GLFW_KEY_X:
					if (app->_isRotAxes.x)
						app->_isRotAxes.x = 0.f;
					else
					{
						app->_isRotAxes.x = 1.f;
						app->_rotations.x = -app->_rotations.x;
					}
					break;
				case GLFW_KEY_Y:
					if (app->_isRotAxes.y)
						app->_isRotAxes.y = 0.f;
					else
					{
						app->_isRotAxes.y = 1.f;
						app->_rotations.y = -app->_rotations.y;
					}
					break;
				case GLFW_KEY_Z:
					if (app->_isRotAxes.z)
						app->_isRotAxes.z = 0.f;
					else
					{
						app->_isRotAxes.z = 1.f;
						app->_rotations.z = -app->_rotations.z;
					}
					break;
				case GLFW_KEY_L:
					app->toggleEditLight();
					app->_red = false;
					app->_green = false;
					app->_blue = false;
					break;
				case GLFW_KEY_R:
					app->_camera.resetPosition();
					app->_rotAngle = vec3(0,0,0);
					break;
			}
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	(void) window;
    glViewport(0, 0, width, height);
}

void Application::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (app)
	    app->_camera.onMouseMove(xposIn, yposIn);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void Application::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (app)
		app->_camera.onMouseScroll(xoffset, yoffset);
}