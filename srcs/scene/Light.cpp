#include "Light.hpp"

Light::Light(vec3 position, vec3 color, float intensity) : _position(position), _intensity(intensity)
{
	changeColor(color);
};

void Light::clampColor(vec3& color)
{
	if (color.x > 1)
		color.x = 1;
	else if (color.x < 0)
		color.x = 0;
	if (color.y > 1)
		color.y = 1;
	else if (color.y < 0)
		color.y = 0;
	if (color.z > 1)
		color.z = 1;
	else if (color.z < 0)
		color.z = 0;
}

void Light::intensityUp(float speed)
{
	_intensity += 0.01 * speed;
}

void Light::intensityDown(float speed)
{
	_intensity -= 0.01 * speed;
	if (_intensity < 0)
		_intensity = 0;
}

void Light::colorUp(Color color, float speed)
{
	switch (color)
	{
		case Color::Red:
			_color.x += 0.01 * speed;
			if (_color.x > 1)
				_color.x = 1;
			break;
		case Color::Green:
			_color.y += 0.01 * speed;
			if (_color.y > 1)
				_color.y = 1;
			break;
		case Color::Blue:
			_color.z += 0.01 * speed;
			if (_color.z > 1)
				_color.z = 1;
			break;
	}
};
void Light::colorDown(Color color, float speed)
{
	switch (color)
	{
		case Color::Red:
			_color.x -= 0.01 * speed;
			if (_color.x < 0)
				_color.x = 0;
			break;
		case Color::Green:
			_color.y -= 0.01 * speed;
			if (_color.y < 0)
				_color.y = 0;
			break;
		case Color::Blue:
			_color.z -= 0.01 * speed;
			if (_color.z < 0)
				_color.z = 0;
			break;
	}
};

void Light::changeColor(const vec3& colors)
{
	_fade.percent = 0.f;
	_fade.start = _color;
	_fade.target = colors;
	clampColor(_fade.target);
	_fade.isActive = true;
}