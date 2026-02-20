#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>

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
		void parseVertex(std::istringstream&);
		void test(); // to remove
	private:
		std::string			_filePath;
		std::vector<Face> 	_faces;
		std::vector<Vertex> _vertices;
};

#endif