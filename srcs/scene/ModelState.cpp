#include "ModelState.hpp"

ModelState::ModelState() = default;
ModelState::~ModelState() = default;

void ModelState::resetTranslation()
{
	_translations = vec3(0,0,0);
};

void ModelState::translate(vec3 v)
{
	_translations += v;
};

void ModelState::scaleUp()
{
	_scale += 0.01;
}

void ModelState::scaleDown()
{
	_scale -= 0.01;
	if (_scale <= 0.f)
		_scale = 0.f;
}

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

void ModelState::toggleTexture()
{
	_blending = -_blending;
	_blend += _blending;
};

void ModelState::applyBlending(float deltaTime)
{
	if (_blend > 0.f && _blend < 1.f)
		_blend += _blending * _blendingSpeed * deltaTime;
	else
		_blend = _blend >= 1.f ? 1.f : 0.f;
};

mat4 ModelState::matrix()
{
	return translationMatrix()
		.mul_mat(rotationMatrix())
		.mul_mat(mat4::scale(_scale));
};

mat4 ModelState::translationMatrix()
{
	mat4 matrix = mat4::identity();
	matrix(3, 0) = _translations.x;
	matrix(3, 1) = _translations.y;
	matrix(3, 2) = _translations.z;
	return matrix;
};

mat4 ModelState::rotationMatrix()
{
	return mat4::rotateX(_rotAngle.x)
		.mul_mat(mat4::rotateY(_rotAngle.y))
		.mul_mat(mat4::rotateZ(_rotAngle.z));
};