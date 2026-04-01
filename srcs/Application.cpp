#include "Application.hpp"

Application::Application(char *file) : _camera(SCR_WIDTH, SCR_HEIGHT), _mesh(std::string(file)) {}
		

Application::~Application()
{
	delete _renderer;
}

void Application::run()
{
	try
	{
		initWindow();
		_renderer = new Renderer();
		_mesh.parse();
		// _mesh.print();
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
	GLMesh glMesh;
	glMesh.upload(_mesh);

	while (!glfwWindowShouldClose(_window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		_deltaTime = currentFrame - _lastFrame;
		_lastFrame = currentFrame;
		// processInput(_window, _camera, _deltaTime);
		glfwSetKeyCallback(_window, Application::keyCallback);
		processInput();

		 // Rendering
		_renderer->beginFrame();
        _renderer->draw(glMesh, _camera);

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
};

void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	(void) scancode /* physical position of a key, not keyboard dependent */; (void) mods; /* bitmask if mod is press, shift, ctrl, alt, super... */

    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app)
		return;

	if (action == GLFW_PRESS) {app->_keys[key] = true;}
    else if (action == GLFW_RELEASE) {app->_keys[key] = false;}

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
			if (action == GLFW_RELEASE)
				app->_renderer->toggleWireframe();
			break;
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