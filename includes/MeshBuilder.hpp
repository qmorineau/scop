#ifndef MESHBUILDER_HPP
#define MESHBUILDER_HPP

#include <vector>
#include <unordered_map>

#include "Mesh.hpp"
#include "Types.hpp"

class MeshBuilder
{
	public:
		class EmptyMesh : public std::runtime_error
		{
			public:
				explicit EmptyMesh(const std::string& msg) : std::runtime_error(msg) {}
		};
		struct VertexIndex
		{
			int vertex = 0;
			int normal = 0;
			int uv = 0;
		};

		struct Face
		{
			bool hasTextCoord = false;
			bool hasNormal = false;
			std::vector<VertexIndex> vertices;
		};

		MeshBuilder(const std::vector<vec3>& p, const std::vector<vec3>& n, const std::vector<vec2>& uv, std::string name);
		MeshBuilder(const MeshBuilder&) = default;
		MeshBuilder& operator=(const MeshBuilder&) = default;

		Mesh build(const std::unordered_map<std::string, Material>&);
		void addFace(Face& f, Material& m);

	private:
		const std::vector<vec3>&							_positions;
		const std::vector<vec3>&							_normals;
		const std::vector<vec2>&							_uvs;
		std::vector<vec3>									_allNormals;
		std::vector<vec2>									_allUvs;
		std::unordered_map<std::string, std::vector<Face>>	_faces;
		Mesh							 					_mesh;

		void addTriangle(VertexIndex&, VertexIndex&, VertexIndex&, const Material&);
		void convertToGpuData(const std::unordered_map<std::string, Material>&);

		int findDuplicateNormal(vec3& v);
		void createNormals();
		void createSmoothNormals();
		void createUvs();

};

#endif