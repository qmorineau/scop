#ifndef LIGHTMANAGER_HPP
#define LIGHTMANAGER_HPP

#include <vector>
#include <stdexcept>

#include "Light.hpp"

class LightManager
{
	public:
		LightManager();
		~LightManager();

		Light*	prev();
		Light*	next();
		Light*	active();

		void	remove();
		void	add(const vec3& pos);
		void	intensityUp();
		void	intensityDown();
		void	colorUp(Color);
		void	colorDown(Color);
		void	changeColor(const vec3&);

		const std::vector<Light*>&	getLights();

	private:
		ssize_t				_activeIndex = -1;
		std::vector<Light*>	_lights;
};

#endif