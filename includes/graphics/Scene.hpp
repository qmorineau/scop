#ifndef SCENE_HPP
#define SCENE_HPP

#include "Types.hpp"
#include "LightManager.hpp"
#include "Camera.hpp"
#include "GLMesh.hpp"

class Scene
{
	public:
		Scene(Mesh& mesh, std::string path);
		~Scene();

		void update(float deltaTime);

		// getter
		Camera&			camera() {return _camera;};
		LightManager&	lights() {return _lights;};
		GLMesh&			mesh() {return _glMesh;};
		const vec3&		rotAngle() const {return _rotAngle;};
		float			blend()	const {return _blend;};
		bool			isLightEditor() const {return _isLightEditor;};
		// setter
		void			setColor(LightManager::ActiveColor c) {_lights.setColor(c);};
		void			toggleLightEditor() {_isLightEditor = -_isLightEditor; _lights.setColor(LightManager::ActiveColor::None);};
		void			toggleTexture() {_blending = -_blending; _blend += _blending;};
		void			resetRot() {_rotAngle = vec3(0,0,0);};
		void			rotateX()
		{
			if (_isRotAxes.x)
				_isRotAxes.x = 0.f;
			else
			{
				_isRotAxes.x = 1.f;
				_rotations.x = -_rotations.x;
			}
		}
		void			rotateY()
		{
			if (_isRotAxes.y)
				_isRotAxes.y = 0.f;
			else
			{
				_isRotAxes.y = 1.f;
				_rotations.y = -_rotations.y;
			}
		}
		void			rotateZ()
		{
			if (_isRotAxes.z)
				_isRotAxes.z = 0.f;
			else
			{
				_isRotAxes.z = 1.f;
				_rotations.z = -_rotations.z;
			}
		}
	private:
		bool			_isLightEditor = false;
		Camera			_camera;
		LightManager	_lights;
		GLMesh			_glMesh;
		// Blending of Texture
		float			_blend;
		float			_blending;
		const float		_rotationSpeed = 100.f;
		const float		_blendingSpeed = 100.f;
		// Rotations
		vec3			_isRotAxes;
		vec3			_rotAngle;
		vec3			_rotations;


		void applyRotation(float deltaTime);
		void applyBlending(float deltaTime);
};

#endif