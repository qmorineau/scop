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
		// Mouse
		if (_inputContext.mouseMoved)
			_inputHandler.handleMouseCallback(this, InputHandler::CommandID::CMD_MOUSE_MOVE);
		if (_inputContext.mouseScrolled)
			_inputHandler.handleMouseCallback(this, InputHandler::CommandID::CMD_MOUSE_SCROLL);
		_inputHandler.handleKeys(this);
		
		// Frame / Delta
		float currentFrame = static_cast<float>(glfwGetTime());
		_deltaTime = currentFrame - _lastFrame;
		_lastFrame = currentFrame;

		// dynamic title
		_window.changeTitle(buildTitle());

		_scene->update(_deltaTime);

		_renderer.beginFrame();
		_renderer.draw(_scene);
		endFrame();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(_window.getWindow());
        glfwPollEvents();
	}
}

void Application::endFrame()
{
	auto& ctx = _inputContext;
    ctx.mouseMoved = false;
    ctx.mouseScrolled = false;
    ctx.mouseOffset.x = 0;
    ctx.mouseOffset.y = 0;
}

std::string Application::buildTitle()
{
	std::string title("Scop");
	const LightManager& lights = this->lights();
	if (isLightEditor())
	{
		switch (lights.getColor())
		{
			case LightManager::ActiveColor::Red:
				title.append(" [Edit Light Mode][RED]");
				break;
			case LightManager::ActiveColor::Green:
				title.append(" [Edit Light Mode][GREEN]");
				break;
			case LightManager::ActiveColor::Blue:
				title.append(" [Edit Light Mode][BLUE]");
				break;
			default:
				title.append(" [Edit Light Mode]");
				break;
		}
	}
	float fps = 1.f / std::max(_deltaTime, 0.0001f);
	title.append(" [" + std::to_string(static_cast<int>(fps)) + " fps]");
	return title;
}