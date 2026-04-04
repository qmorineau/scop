#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <string>
#include <iostream>

#include "Shader.hpp"
#include "Types.hpp"

struct Material
{
	std::string		_name;
	float			_Ns = 32; // Specular Exponent
	vec3			_Ka = vec3(0.2f, 0.2f, 0.2f); // Ambiant Color
	vec3			_Kd = vec3(0.8f, 0.8f, 0.8f); // Diffuse Color
	vec3			_Ks; // Specular Color
	vec3			_Ke; // Emissive Color
	float			_Ni = 1.0f; // Optical Density
	float			_d = 1.0f; // Transparency
	std::string 	_map_Kd;
	int				_illum = 2; // illumination mod
	bool			_hasTexture = false;

	Material(std::string name) : _name(name) {};

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

	void apply(Shader& shader) const
	{
		shader.setFloat("material.Ns", _Ns);
		shader.setVec3("material.Ka", _Ka);
		shader.setVec3("material.Kd", _Kd);
		shader.setVec3("material.Ks", _Ks);
		shader.setBool("material.hasTexture", _hasTexture);
		if (_hasTexture)
		{
			// glActiveTexture(GL_TEXTURE0);
			// glBindTexture(GL_TEXTURE_2D, textureID);
			// shader.setInt("material_mapKd", 0);
		}
		// shader.setVec3("material.Ke", _Ke);
		// shader.setFloat("material.Ni", _Ni);
		// shader.setFloat("material.d", _d);
		// shader.setInt("material.illum", _illum);
	};
};
#endif