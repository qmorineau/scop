#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>

#include "Types.hpp"

class Mesh
{
	public:
		std::vector<Vertex> _vertices;
		std::vector<uint32_t> _indices;

		Mesh();
		Mesh(const Mesh&);
		Mesh(const std::string& file);
		Mesh& operator=(const Mesh&);
		~Mesh();

		void parse();
		void render();
		void print();
	private:
		void parseVertexNormal(std::istringstream& iss);
		void parsePosition(std::istringstream&);
		void parseTexture(std::istringstream&);
		void createFace(std::istringstream& iss);
		void parseVertex(std::string vertex);
		int	findDuplicateVertex(Vertex& v);

		std::vector<Vector3> _positions;
		std::vector<Vector3> _normals;
		std::vector<Vector2> _textures;
		std::string			_filePath;
};

#endif