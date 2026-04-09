#include "Light.hpp"

Light::Light(vec3 position, vec3 color, float intensity) : _position(position), _intensity(intensity)
{
	changeColor(color);
};

void Light::clampColor()
{
	if (_color.x > 1)
		_color.x = 1;
	else if (_color.x < 0)
		_color.x = 0;
	if (_color.y > 1)
		_color.y = 1;
	else if (_color.y < 0)
		_color.y = 0;
	if (_color.z > 1)
		_color.z = 1;
	else if (_color.z < 0)
		_color.z = 0;
}

void Light::intensityUp()
{
	_intensity += 0.01;
}

void Light::intensityDown()
{
	_intensity -= 0.01;
	if (_intensity < 0)
		_intensity = 0;
}

void Light::colorUp(Color color)
{
	switch (color)
	{
		case Color::Red:
			_color.x += 0.01;
			if (_color.x > 1)
				_color.x = 1;
			break;
		case Color::Green:
			_color.y += 0.01;
			if (_color.y > 1)
				_color.y = 1;
			break;
		case Color::Blue:
			_color.z += 0.01;
			if (_color.z > 1)
				_color.z = 1;
			break;
	}
};
void Light::colorDown(Color color)
{
	switch (color)
	{
		case Color::Red:
			_color.x -= 0.01;
			if (_color.x < 0)
				_color.x = 0;
			break;
		case Color::Green:
			_color.y -= 0.01;
			if (_color.y < 0)
				_color.y = 0;
			break;
		case Color::Blue:
			_color.z -= 0.01;
			if (_color.z < 0)
				_color.z = 0;
			break;
	}
};

void Light::changeColor(const vec3& colors)
{
	_color = colors;
	clampColor();
}