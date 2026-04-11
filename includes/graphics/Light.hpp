#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Types.hpp"

enum class Color
{
	Red,
	Green,
	Blue
};

class Light
{
	public:
		Light(vec3 position, vec3 color, float intensity);

		void colorUp(Color color, float speed);
		void colorDown(Color color, float speed);
		void intensityUp(float speed);
		void intensityDown(float speed);
		void changeColor(const vec3& colors);

		vec3 getPosition() {return _position;};
		vec3 getColor() {return _color;};
		float getIntensity() {return _intensity;};

	private:
		vec3	_position;
		vec3	_color;
		float	_intensity;

		void clampColor();
};

#endif