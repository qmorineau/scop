#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <string>

#include "Material.hpp"
#include "Types.hpp"

class Mesh
{
	public:
		Mesh(std::string name) : _name(name) {};
		
		void addVertex(Vertex& v) {_vertices.push_back(v);};

	private:
		std::string				_name;
		std::vector<Vertex>		_vertices;
		std::vector<uint32_t>	_indices;
};

#endif