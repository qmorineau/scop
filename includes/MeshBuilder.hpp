#ifndef MESHBUILDER_HPP
#define MESHBUILDER_HPP

#include <vector>
#include <unordered_map>

#include "Mesh.hpp"
#include "Types.hpp"

class MeshBuilder
{
	public:
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

		MeshBuilder(const std::vector<vec3>& p, const std::vector<vec3>& n, const std::vector<vec2>& uv);

		void build();
		void addMesh(std::string name);
		void addFace(Face& f, Material& m);
		std::vector<Mesh> getMeshes() {return _meshes;};

	private:
		const std::vector<vec3>&	_positions;
		const std::vector<vec3>&	_normals;
		const std::vector<vec2>&	_uvs;

		std::unordered_map<std::string, std::vector<Face>>	_faces;

		std::vector<Mesh>			_meshes;
};

#endif