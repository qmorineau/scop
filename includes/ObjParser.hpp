#ifndef OBJPARSER_HPP
#define OBJPARSER_HPP

#include <memory>
#include <vector>
#include <cfloat>  
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>

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

		std::vector<Mesh> getMeshes() {return _meshes;};

	private:
		class Face
		{
			std::vector<int> index;
		};
		class ObjectData
		{
			std::string name;
			std::unordered_map<std::string, std::vector<Face>> faces;
		};

		std::string									_pathFile;
		std::ifstream								_file;
		std::string									_actualMaterial;
		std::vector<Vertex> 						_vertices;
		std::vector<uint32_t> 						_indices;
		std::vector<vec3>	 						_positions;
		std::vector<vec3>	 						_normals;
		std::vector<vec2>	 						_uvs;
		std::unordered_map<std::string, Material>	_materials;
		std::unique_ptr<MeshBuilder>				_builder;
		std::vector<Mesh>							_meshes;

		using Handler = void (ObjParser::*)(std::istringstream&);
 		static const std::unordered_map<std::string, Handler> handlers; // Dispatch Table

		void parse();
		void build();
		
		void parsePosition(std::istringstream&);
		void parseUvs(std::istringstream&);
		void parseVertexNormal(std::istringstream&);
		void parseObjectName(std::istringstream&);
		void createFace(std::istringstream&);
		void parseMtlFile(std::istringstream&);
		void parseSmoothing(std::istringstream&);
		void useMtl(std::istringstream&);
		MeshBuilder::VertexIndex parseVertex(const std::string&);

		void centerMeshes();

		int findDuplicateNormal(vec3&);
		void createNormals();
		void createUvs();
};

#endif