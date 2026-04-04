#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <string>

#include "Material.hpp"
#include "Types.hpp"

struct Mesh
{
	public:
		Mesh(std::string name) : _name(name) {};
		std::string				_name;
		std::vector<Vertex>		_vertices;
		std::vector<uint32_t>	_indices;
		
		void addVertex(Vertex& v);
	private:

		int findDuplicateVertex(Vertex& v);
};

#endif