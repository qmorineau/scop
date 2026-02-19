#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>
#include <string>

class Face;
class Vertex;

class Object
{
	public:
		Object();
		Object(const Object&);
		Object(const std::string);
		Object& operator=(const Object&);
		~Object();
	private:
		std::vector<Face> faces;
		std::vector<Vertex> vertices;
};

#endif