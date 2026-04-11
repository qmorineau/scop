#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <string>
#include <iostream>

#include "Types.hpp"

class Shader;

struct Material
{
	std::string		_name;
	float			_Ns = 32; 						// Specular Exponent
	vec3			_Ka = vec3(0.2f, 0.2f, 0.2f); 	// Ambiant Color
	vec3			_Kd = vec3(0.8f, 0.8f, 0.8f); 	// Diffuse Color
	vec3			_Ks = vec3(1.f, 1.f, 1.f); 		// Specular Color
	vec3			_Ke; 							// Emissive Color
	float			_Ni = 1.0f; 					// Optical Density
	float			_d = 1.0f; 						// Transparency
	std::string	 	_map_Kd;						// File of texture
	int				_illum = 2; 					// illumination mod
	bool			_hasTexture = false;
	int				_id;							// Id of texture
	int				_triPlanarId;					// Id of default texture

	Material(std::string name) : _name(name) {};

	void apply(Shader& shader) const;
};
#endif