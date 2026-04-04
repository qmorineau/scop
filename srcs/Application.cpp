#include "Application.hpp"

Application::Application(char *file) : _camera(SCR_WIDTH, SCR_HEIGHT), _renderer(nullptr), _parser(std::string(file))
{
	_lights.push_back(new Light(vec3(5,5,5), vec3(1,0,0), 2));
	_lights.push_back(new Light(vec3(-5,5,5), vec3(0,1,0), 2));
	_lights.push_back(new Light(vec3(-5,-5,5), vec3(0,0,1), 2));
}
		

Application::~Application()
{
	if (_renderer)
		delete _renderer;
	for (auto l : _lights)
		delete l;
}

void Application::run()
{
	try
	{
		initWindow();
		_renderer = new Renderer(_parser.getMeshes());
		renderLoop();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
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

		 // Rendering
		_renderer->beginFrame();
		_renderer->draw(_camera, _lights);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(_window);
        glfwPollEvents();
	}
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
// void Application::processInput(GLFWwindow *window, Camera &camera, const float deltaTime)

void Application::processInput()
{
	if (_keys[GLFW_KEY_W])
		_camera.ProcessKeyboard(FORWARD, _deltaTime);
	if (_keys[GLFW_KEY_S])
		_camera.ProcessKeyboard(BACKWARD, _deltaTime);
	if (_keys[GLFW_KEY_A])
		_camera.ProcessKeyboard(LEFT, _deltaTime);
	if (_keys[GLFW_KEY_D])
		_camera.ProcessKeyboard(RIGHT, _deltaTime);
	if (_editLight)
	{
		if (_keys[GLFW_KEY_0])
		{
			if (_red)
				for (auto light : _lights)
					light->increaseColor(Color::Red);
			if (_green)
				for (auto light : _lights)
					light->increaseColor(Color::Green);
			if (_blue)
				for (auto light : _lights)
					light->increaseColor(Color::Blue);
		}
		if (_keys[GLFW_KEY_9])
		{
			if (_red)
				for (auto light : _lights)
					light->decreaseColor(Color::Red);
			if (_green)
				for (auto light : _lights)
					light->decreaseColor(Color::Green);
			if (_blue)
				for (auto light : _lights)
					light->decreaseColor(Color::Blue);
		}
	}

};

void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	(void) scancode /* physical position of a key, not keyboard dependent */; (void) mods; /* bitmask if mod is press, shift, ctrl, alt, super... */

    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app)
		return;

	if (action == GLFW_PRESS) {app->_keys[key] = true;}
    else if (action == GLFW_RELEASE) {app->_keys[key] = false;}

	if (action == GLFW_RELEASE)
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
				app->_renderer->setMode(RenderMode::Texture);
				break;
			case GLFW_KEY_3:
				app->_renderer->setMode(RenderMode::Face);
				break;
			case GLFW_KEY_P:
				app->_renderer->toggleWireframe();
				break;
			case GLFW_KEY_L:
				app->toggleEditLight();
				app->_red = false;
				app->_green = false;
				app->_blue = false;
				break;
			case GLFW_KEY_R:
				if (app->_editLight)
				{
					app->_red = true;
					app->_green = false;
					app->_blue = false;
				}
				break;
			case GLFW_KEY_G:
				if (app->_editLight)
				{
					app->_red = false;
					app->_green = true;
					app->_blue = false;
				}
				break;
			case GLFW_KEY_B:
				if (app->_editLight)
				{
					app->_red = false;
					app->_green = false;
					app->_blue = true;
				}
				break;
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	(void) window;
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
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