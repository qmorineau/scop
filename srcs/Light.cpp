#include "Light.hpp"

Light::Light(vec3 position, vec3 color, float intensity) : _position(position), _intensity(intensity)
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
	_color = color;
};

void Light::increaseColor(LightColor color)
{
	switch (color)
	{
		case LightColor::Red:
			_color.x += 0.05;
			if (_color.x > 1)
				_color.x = 1;
			break;
		case LightColor::Green:
			_color.y += 0.05;
			if (_color.y > 1)
				_color.y = 1;
			break;
		case LightColor::Blue:
			_color.z += 0.05;
			if (_color.z > 1)
				_color.z = 1;
			break;
	}
};
void Light::decreaseColor(LightColor color)
{
	switch (color)
	{
		case LightColor::Red:
			_color.x -= 0.05;
			if (_color.x < 0)
				_color.x = 0;
			break;
		case LightColor::Green:
			_color.y -= 0.05;
			if (_color.y < 0)
				_color.y = 0;
			break;
		case LightColor::Blue:
			_color.z -= 0.05;
			if (_color.z < 0)
				_color.z = 0;
			break;
	}
};