#include "Application.hpp"

Application::Application(char *file) :
	_parser(std::string(file)),
	_window(this),
	_scene(nullptr)
{
	_scene = new Scene(_parser.getMeshes(), _parser.getPath());
}
		

Application::~Application()
{
	if (_scene)
		delete _scene;
}

void Application::run()
{		
	renderLoop();
}

void Application::renderLoop()
{
	// glfwSwapInterval(0); // disable vsync
	glfwSwapInterval(1); // cap framerate to monitor framerate

	while (!glfwWindowShouldClose(_window.getWindow()))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		_deltaTime = currentFrame - _lastFrame;
		_lastFrame = currentFrame;

		_window.manageTitle(*this);
		glfwSetKeyCallback(_window.getWindow(), InputManager::keyCallback);
		_inputManager.handleKeys(this);
		_scene->update(_deltaTime);

		_renderer.beginFrame();
		_renderer.draw(_scene);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(_window.getWindow());
        glfwPollEvents();
	}
}
