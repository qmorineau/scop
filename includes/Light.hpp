#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Types.hpp"

enum class LightColor
{
	Red,
	Green,
	Blue
};

class Light
{
	public:
		Light(vec3 position, vec3 color, float intensity);

		void increaseColor(LightColor color);
		void decreaseColor(LightColor color);

	private:
		vec3	_position;
		vec3	_color;
		float	_intensity;


		vec3 getPosition() {return _position;};
		vec3 getColor() {return _color;};
		float getIntensity() {return _intensity;};
};

#endif