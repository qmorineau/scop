#ifndef MODELSTATE_HPP
#define MODELSTATE_HPP

#include "Types.hpp"

class ModelState
{
	public:
		ModelState() = default;
		~ModelState() = default;

		void applyRotation(float deltaTime);
		void resetTranslation();
		void translateX();
		void translateY();
		void translateZ();
		void resetRotation();
		void rotateX();
		void rotateY();
		void rotateZ();

	private:
		vec3	_offsets;
		vec3	_translations;
		vec3	_rotationsEnable;
		vec3	_rotAngle;
		vec3	_rotations;
		float 	_rotationSpeed;
};

#endif