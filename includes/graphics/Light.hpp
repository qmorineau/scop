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
		struct LightFade
		{
			vec3	start;
			vec3	target;
			float	percent = 0.f;
			float	duration = 0.5f;
			bool	isActive = false;
		};
		Light(vec3 position, vec3 color, float intensity);
		void changeColor(const vec3& colors);

		// setter
		void colorUp(Color color, float speed);
		void colorDown(Color color, float speed);
		void intensityUp(float speed);
		void intensityDown(float speed);
		void setColor(const vec3& color) {_color = color;};

		// getter
		LightFade&	fade() {return _fade;};
		vec3 		getPosition() {return _position;};
		vec3 		getColor() {return _color;};
		float 		getIntensity() {return _intensity;};

	private:
		vec3		_position;
		vec3		_color;
		float		_intensity;
		LightFade	_fade;

		void clampColor(vec3& color);
};

#endif