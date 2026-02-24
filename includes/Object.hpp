#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>

#include "Types.hpp"

class Face;

class Object
{
	public:
		Object();
		Object(const Object&);
		Object(const std::string& file);
		Object& operator=(const Object&);
		~Object();

		void parse();
		void render();
		void test(); // to remove
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
		std::vector<uint32_t> _indices;

		std::string			_filePath;
		std::vector<Vertex> _vertices;
		
};

#endif