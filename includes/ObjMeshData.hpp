#ifndef OBJMESHDATA_HPP
#define OBJMESHDATA_HPP

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdint>
#include <algorithm>

#include "ObjMeshData.hpp"
#include "Types.hpp"
#include "Math.hpp"

class ObjMeshData
{
	struct VertexIndex
	{
		int vertex = 0;
		int normal = 0;
		int textCoord = 0;
	};

	struct Face
	{
		bool hasTextCoord = false;
		bool hasNormal = false;
		std::vector<VertexIndex> vertices;
	};

	public:
		std::vector<Vertex> _vertices;
		std::vector<uint32_t> _indices;

		ObjMeshData();
		ObjMeshData(const ObjMeshData&);
		ObjMeshData(const std::string& file);
		ObjMeshData& operator=(const ObjMeshData&);
		~ObjMeshData();

		void parse();
		void print();
	private:
		void parseVertexNormal(std::istringstream& iss);
		void parsePosition(std::istringstream&);
		void parseTexture(std::istringstream&);
		VertexIndex parseVertex(const std::string& vertex);
		void createFace(std::istringstream& iss);
		int  findDuplicatePosition(vec3& v);
		int  findDuplicateNormal(vec3& v);
		int	 findDuplicateVertex(Vertex& v);
		void centerMesh();
		void createNormal();
		void createTextCoord();
		void createTextureCoord();
		void convertToGpuData();
		void addTriangle(VertexIndex a, VertexIndex b, VertexIndex c);

		std::vector<Vector3> _positions;
		std::vector<Vector3> _normals;
		std::vector<Vector2> _textures;
		std::vector<Face>	_faces;
		std::string			_filePath;
};

#endif