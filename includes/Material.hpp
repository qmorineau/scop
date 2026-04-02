#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <string>
#include <iostream>

#include "Types.hpp"

struct Material
{
	float 	_Ns; // Specular Exponent
	vec3 _Ka; // Ambiant Color
	vec3 _Kd; // Diffuse Color
	vec3 _Ks; // Specular Color
	vec3 _Ke; // Emissive Color
	float _Ni; // Optical Density
	float _d; // Transparency
	std::string _map_Kd;
	int		_illum; // illumination mod

	void print()
	{
		std::cout << "Material :" << std::endl;
		std::cout << "Ns: " << _Ns << std::endl;
		std::cout << "Ka: " << _Ka << std::endl;
		std::cout << "Kd: " << _Kd << std::endl;
		std::cout << "Ks: " << _Ks << std::endl;
		std::cout << "Ke: " << _Ke << std::endl;
		std::cout << "Ni: " << _Ni << std::endl;
		std::cout << "d: " << _d << std::endl;
		std::cout << "map_Kd: " << _map_Kd << std::endl;
		std::cout << "illum: " << _illum << std::endl;
	}
};
#endif