#ifndef SCENE_HPP
#define SCENE_HPP

#include "Types.hpp"
#include "LightManager.hpp"
#include "Camera.hpp"
#include "MeshGPU.hpp"

class Scene
{
	public:
		Scene(Mesh& mesh, std::string path);
		~Scene();

		void update(float deltaTime);

		// getter
		Camera&			camera() {return _camera;};
		LightManager&	lights() {return _lights;};
		MeshGPU&		mesh() {return _MeshGPU;};
		const vec3&		getRotAngle() const {return _rotAngle;};
		float			getBlend()	const {return _blend;};
		bool			isLightEditor() const {return _isLightEditor;};
		// setter
		void			setColor(LightManager::ActiveColor c) {_lights.setColor(c);};
		void			toggleLightEditor();
		void			toggleTexture();
		void			resetRot() {_rotAngle = vec3(0,0,0);};
		void			rotateX();
		void			rotateY();
		void			rotateZ();
	private:
		bool			_isLightEditor = false;
		Camera			_camera;
		LightManager	_lights;
		MeshGPU			_MeshGPU;
		// Blending of Texture
		float			_blend;
		float			_blending;
		const float		_rotationSpeed = 100.f;
		const float		_blendingSpeed = 100.f;
		// Rotations
		vec3			_rotationsEnable;
		vec3			_rotAngle;
		vec3			_rotations;


		void applyRotation(float deltaTime);
		void applyBlending(float deltaTime);
};

#endif