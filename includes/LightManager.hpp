#ifndef LIGHTMANAGER_HPP
#define LIGHTMANAGER_HPP

#include <vector>
#include <stdexcept>

#include "Light.hpp"


class LightManager
{
	public:
		enum class ActiveColor
		{
			Red,
			Green,
			Blue,
			None
		};
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
		ActiveColor getColor() {return _activeColor;};
		void		setColor(ActiveColor c) {_activeColor = c;};
	private:
		ssize_t				_activeIndex = -1;
		std::vector<Light*>	_lights;
		ActiveColor			_activeColor = ActiveColor::None;
};

#endif