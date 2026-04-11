#ifndef OBJPARSER_HPP
#define OBJPARSER_HPP

#include <memory>
#include <vector>
#include <cfloat>  
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>

#include "MtlParser.hpp"
#include "MeshBuilder.hpp"
#include "Mesh.hpp"

class ObjParser
{
	class ParseError : public std::runtime_error
	{
		public:
			explicit ParseError(const std::string& msg) : std::runtime_error(msg) {}
	};
	public:
		ObjParser(std::string file);
		~ObjParser() {};

		Mesh& getMeshes() {return _mesh;};
		std::string getPath() {return _pathFolder;};
	private:
		struct ObjectData
		{
			std::string name;
			std::unordered_map<std::string, std::vector<MeshBuilder::Face>> faces;
		};

		std::string									_pathFile;
		std::string									_pathFolder;
		std::ifstream								_file;

		std::vector<vec3>	 						_positions;
		std::unordered_map<vec3, int,
			vec3::Vec3Hash, vec3::Vec3Eq>			_positionsIdx;
		std::vector<vec3>	 						_normals;
		std::unordered_map<vec3, int,
			vec3::Vec3Hash, vec3::Vec3Eq>			_normalsIdx;
		std::vector<vec2>	 						_uvs;
		std::unordered_map<vec2, int,
			vec2::Vec2Hash, vec2::Vec2Eq>			_uvsIdx;

		ObjectData									_rawData;
		std::unordered_map<std::string, Material>	_materials;
		Mesh										_mesh;


		int											idxActualObject = -1;
		std::vector<int>							idxActualGroups;
		int											idxActualMaterial = 0;
		int											actualSmoothingGroup = 0;

		std::vector<std::string>					objectNames;
		std::unordered_map<std::string, int>		objectIndices;
		std::vector<std::string>					groupNames;
		std::unordered_map<std::string, int>		groupIndices;
		std::vector<std::string>					materialNames;
		std::unordered_map<std::string, int>		materialIndices;


		using Handler = void (ObjParser::*)(std::istringstream&);
 		static const std::unordered_map<std::string, Handler> handlers; // Dispatch Table

		void parse();
		void build();
		
		void parsePosition(std::istringstream&);
		void parseUvs(std::istringstream&);
		void parseVertexNormal(std::istringstream&);
		void parseObjectName(std::istringstream&);
		void parseGroupName(std::istringstream&);
		void parseParameterSpaceVertex(std::istringstream&);
		void createFace(std::istringstream&);
		void createLine(std::istringstream&);
		void parseMtlFile(std::istringstream&);
		void parseSmoothing(std::istringstream&);
		void useMtl(std::istringstream&);
		MeshBuilder::VertexIndex parseVertex(const std::string&);

		void centerPositions();
		void normalizePositions();

		int findDuplicateNormal(vec3&);
		void createNormals();
		void createUvs();
		void printRawData();
};

#endif