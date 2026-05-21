#ifndef INPUTCONTEXTHPP
#define INPUTCONTEXTHPP

#include <bitset>
#include <algorithm>
#include <vector>

#include "Types.hpp"

class InputContext
{
	public:
		InputContext();
		~InputContext();

		void pressKey(int key);
		void releaseKey(int key);
		void pressMouse(int key);
		void releaseMouse(int key);

		// setter
		void setMods(int m)					{_mods = m;};
		void setIsMouseCaptured(bool v)	{_isMouseCaptured = v;};
		void setIsMouseMoved(bool v) 	{_isMouseMoved = v;};
		void setIsMouseScrolled(bool v)	{_isMouseScrolled = v;};
		void setMouseScrolled(vec2 v) 	{_mouseScroll = v;};
		void setMousePos(vec2 v) 		{_mousePos = v;};
		// getter
		int getMods()					{return _mods;};
		bool isMouseCaptured() 			{return _isMouseCaptured;};
		bool isMouseMoved()				{return _isMouseMoved;};
		bool isMouseScrolled()			{return _isMouseScrolled;};
		const vec2& mouseScroll()		{return _mouseScroll;};
		const vec2& mousePos()			{return _mousePos;};
		const auto& activeKeys()		{return _activeKeys;};
	private:
		int					_mods;
		bool				_isMouseCaptured = true;
		std::bitset<1024>	_keys;
		std::vector<int>	_activeKeys;
		std::bitset<8>		_mouse;
		vec2				_mousePos;
		bool				_isMouseMoved = false;
		vec2				_mouseScroll;
		bool				_isMouseScrolled = false;
};

#endif