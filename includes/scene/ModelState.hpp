#ifndef MODELSTATE_HPP
#define MODELSTATE_HPP

#include "Matrix4.hpp"
#include "Types.hpp"

class ModelState
{
	public:
		ModelState() = default;
		~ModelState() = default;

		mat4 matrix();

		void applyRotation(float deltaTime);
		void applyBlending(float deltaTime);

		void resetTranslation();
		void translate(vec3 v);
		
		void resetRotation();
		void rotateX();
		void rotateY();
		void rotateZ();

		void toggleTexture();

		// getter
		float getBlend() {return _blend;};
	private:
		// translation
		vec3		_translations;
		// rotation
		vec3		_rotationsEnable = vec3(0, 1, 0);
		vec3		_rotAngle;
		vec3		_rotations = vec3(0.01, 0.01, 0.01);
		const float	_rotationSpeed = 100.f;
		// texture blending
		float 		_blend = 0.f;
		float		_blending = -0.01f;
		const float	_blendingSpeed = 100.f;

		mat4 translationMatrix();
		mat4 rotationMatrix();
};

#endif