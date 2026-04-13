#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>

class Application;
class LightManager;

class InputManager
{
	public:
		InputManager() {};
		~InputManager() {};

		void		handleKeys(Application* app);
		void		setKey(int key, bool state) {_keys[key] = state;};
		// Callbacks
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouseCallback(GLFWwindow* window, double xposIn, double yposIn);
		static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

		// Dispatch Tables
		using Handler = void (InputManager::*)(Application*);
 		static const std::unordered_map<int, Handler> lightHandlers;
		static const std::unordered_map<int, Handler> handlers;

		// Handlers
		void closeWindow(Application *);
		void editRed(Application *);
		void editGreen(Application *);
		void editBlue(Application *);
		void addLight(Application *);
		void deleteLight(Application *);
		void toggleLightMod(Application *);
		void prevLight(Application *);
		void nextLight(Application *);
		void setPhong(Application *);
		void setFace(Application *);
		void setMaterial(Application *);
		void applyTexture(Application *);
		void rotateX(Application *);
		void rotateY(Application *);
		void rotateZ(Application *);
		void resetCam(Application *);
		void changeFaceRendering(Application *);
		void changeMode(Application *);

	private:
		bool _keys[1024] = {false};
		void lightEditor(LightManager& manager, float deltaTime);

};

#endif