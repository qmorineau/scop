#include "ModelState.hpp"

void ModelState::resetTranslation()
{
	_offsets = vec3(0,0,0);
};

void ModelState::translateX()
{
	_offsets.x += _translations.x;
};

void ModelState::translateY()
{
	_offsets.y += _translations.y;
};

void ModelState::translateZ()
{
	_offsets.z += _translations.z;
};

void ModelState::applyRotation(float deltaTime)
{
	if (_rotationsEnable.x)
		_rotAngle.x += _rotations.x * _rotationSpeed * deltaTime;
	if (_rotationsEnable.y)
		_rotAngle.y += _rotations.y * _rotationSpeed * deltaTime;
	if (_rotationsEnable.z)
		_rotAngle.z += _rotations.z * _rotationSpeed * deltaTime;
};

void ModelState::resetRotation()
{
	_rotAngle = vec3(0,0,0);
};

void ModelState::rotateX()
{
	if (_rotationsEnable.x)
		_rotationsEnable.x = 0.f;
	else
	{
		_rotationsEnable.x = 1.f;
		_rotations.x = -_rotations.x;
	}
};

void ModelState::rotateY()
{
	if (_rotationsEnable.y)
		_rotationsEnable.y = 0.f;
	else
	{
		_rotationsEnable.y = 1.f;
		_rotations.y = -_rotations.y;
	}
};

void ModelState::rotateZ()
{
	if (_rotationsEnable.z)
		_rotationsEnable.z = 0.f;
	else
	{
		_rotationsEnable.z = 1.f;
		_rotations.z = -_rotations.z;
	}
};
