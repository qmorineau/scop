#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <iostream>
#include <string>
#include <exception>

#include <glad/glad.h>
#include <GLFW/glfw3.h> 

#include "InputManager.hpp"
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
		void closeWindow() {_window.closeWindow();};

		// getter
		bool 			isLightEditor() {return _scene->isLightEditor();};
		void			toggleLightEditor() {_scene->toggleLightEditor();};
		float			getDelta() {return _deltaTime;};
		LightManager&	lights() {return _scene->lights();};
		Camera&			getCamera() {return _scene->camera();};
		Renderer&		renderer() {return _renderer;};
		Scene*			scene() {return _scene;};
		InputManager&	inputManager() {return _inputManager;};
		// setter
		void 			setKey(int key, bool state) {_inputManager.setKey(key, state);};
		void 			setColor(LightManager::ActiveColor c) {_scene->setColor(c);};

	private:
		ObjParser			_parser;
		Window				_window;
		InputManager		_inputManager;
		Scene*				_scene;
		Renderer			_renderer;
		float				_deltaTime = 0.0f;
		float				_lastFrame = 0.0f;

		void initWindow();
		void applyRotation();
		void renderLoop();
		void manageWindowTitle();
};

#endif