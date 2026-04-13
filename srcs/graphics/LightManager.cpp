#include "LightManager.hpp"

LightManager::LightManager()
{
	add(vec3(0, 1, 1));
}

LightManager::~LightManager()
{
	for (auto light : _lights)
		delete light;
}

Light*	LightManager::prev()
{
	if (_activeIndex < 0)
		return nullptr;
	_activeIndex--;
	if (_activeIndex < 0)
	{
		_activeIndex = _lights.size() - 1;
		return _lights[_lights.size() - 1];
	}
	return (_lights[_activeIndex]);
};

Light*	LightManager::next()
{
	if (_activeIndex < 0)
		return nullptr;
	_activeIndex++;
	if (_activeIndex >= static_cast<ssize_t>(_lights.size()))
	{
		_activeIndex = 0;
		return _lights[0];
	}
	return (_lights[_activeIndex]);
};

Light*	LightManager::active()
{
	if (_activeIndex < 0)
		return nullptr;
	return _lights[_activeIndex];
};

void LightManager::add(const vec3& pos)
{
	Light* newLight = new Light(pos, vec3(1, 1, 1), 0.5);
	_lights.push_back(newLight);
	_activeIndex = _lights.size() - 1;
};

void LightManager::remove()
{
	if (_activeIndex < 0)
		return;
	delete _lights[_activeIndex];
	_lights.erase(_lights.begin() + _activeIndex);
	if (!_lights.size())
		_activeIndex = -1;
	else if (_activeIndex >= static_cast<ssize_t>(_lights.size()))
		_activeIndex = _lights.size() -1;
};

void LightManager::intensityUp(float deltaTime)
{
	float speed = deltaTime * _changeSpeed;
	if (_activeIndex < 0)
		return;
	_lights[_activeIndex]->intensityUp(speed);
};

void LightManager::intensityDown(float deltaTime)
{
	float speed = deltaTime * _changeSpeed;
	if (_activeIndex < 0)
		return;
	_lights[_activeIndex]->intensityDown(speed);
};

void LightManager::colorUp(Color color, float deltaTime)
{
	float speed = deltaTime * _changeSpeed;
	if (_activeIndex < 0)
		return;
	_lights[_activeIndex]->colorUp(color, speed);
};

void LightManager::colorDown(Color color, float deltaTime)
{
	float speed = deltaTime * _changeSpeed;
	if (_activeIndex < 0)
		return;
	_lights[_activeIndex]->colorDown(color, speed);
};

void LightManager::changeColor(const vec3& colors)
{
	if (_activeIndex < 0)
		return;
	_lights[_activeIndex]->changeColor(colors);
}

const std::vector<Light*>& LightManager::getLights()
{
	return _lights;
}