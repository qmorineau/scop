#ifndef KEYHANDLER_HPP
#define KEYHANDLER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>

class Application;
class LightManager;

class KeyHandler
{
	public:
		KeyHandler() {};
		~KeyHandler() {};

		void		handleKeys(Application* app);
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void		setKey(int key, bool state) {_keys[key] = state;};

		// Dispatch Tables
		using Handler = void (KeyHandler::*)(Application*);
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