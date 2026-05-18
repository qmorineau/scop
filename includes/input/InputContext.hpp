#ifndef INPUTCONTEXTHPP
#define INPUTCONTEXTHPP

#include "Types.hpp"

struct InputContext
{
	bool	keys[1024] = {false};
	bool	mouse[8] = {false};
	vec2	mousePos;
	bool	mouseMoved = false;
	vec2	mouseOffset;
	bool	mouseScrolled = false;
	vec2	ndc; // normalize device coordinate
};

#endif