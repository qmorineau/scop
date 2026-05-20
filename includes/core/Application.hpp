#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <iostream>
#include <string>
#include <exception>
#include <algorithm>

#include <glad/glad.h>
#include <GLFW/glfw3.h> 

#include "InputManager.hpp"
#include "InputHandler.hpp"
#include "InputContext.hpp"
#include "ObjParser.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#include "Scene.hpp"

class Application
{
	public:
		Application(char *file);
		~Application();
		
		void run();

		// getter
		bool 			isLightEditor() 	{return _scene->isLightEditor();};
		void			toggleLightEditor() {_scene->toggleLightEditor();};
		Window&			window() 			{return _window;};
		float			getDelta() 			{return _deltaTime;};
		LightManager&	lights() 			{return _scene->lights();};
		Camera&			getCamera() 		{return _scene->camera();};
		Renderer&		renderer() 			{return _renderer;};
		Scene*			scene() 			{return _scene;};
		InputManager&	inputManager()		{return _inputManager;};
		InputHandler&	inputHandler()		{return _inputHandler;};
		InputContext&	inputContext()		{return _inputContext;};
		// setter
		void 			setColor(LightManager::ActiveColor c) {_scene->setColor(c);};

	private:
		ObjParser			_parser;
		Window				_window;
		InputManager		_inputManager;
		InputHandler		_inputHandler;
		InputContext		_inputContext;
		Scene*				_scene;
		Renderer			_renderer;
		float				_deltaTime = 0.0f;
		float				_lastFrame = 0.0f;

		void 		renderLoop();
		void 		endFrame();
		std::string buildTitle();
};

#endif