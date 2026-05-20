#ifndef SCENE_HPP
#define SCENE_HPP

#include "Types.hpp"
#include "LightManager.hpp"
#include "Camera.hpp"
#include "Model.hpp"

class Scene
{
	public:
		Scene(Mesh& mesh, std::string path);
		~Scene();

		void update(float deltaTime);

		// getter
		Camera&			camera() {return _camera;};
		LightManager&	lights() {return _lights;};
		bool			isLightEditor() const {return _isLightEditor;};
		Model&			model() {return _model;};
		// setter
		void			setColor(LightManager::ActiveColor c) {_lights.setColor(c);};
		void			toggleLightEditor();
	private:
		bool			_isLightEditor = false;
		Camera			_camera;
		LightManager	_lights;
		Model			_model;
};

#endif