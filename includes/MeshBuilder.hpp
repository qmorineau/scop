#ifndef MESHBUILDER_HPP
#define MESHBUILDER_HPP

#include <vector>
#include <unordered_map>

#include "Mesh.hpp"
#include "Types.hpp"

const vec3 faceColors[4] =
{
	vec3(0.0f, 0.0f, 0.0f),
	vec3(0.25f, 0.25f, 0.25f),
	vec3(0.5f, 0.5f, 0.5f),
	vec3(0.75f, 0.75f, 0.75f)
};

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
			int textCoord = 0;
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

		Mesh build();
		void addFace(Face& f, Material& m);

	private:
		const std::vector<vec3>&							_positions;
		const std::vector<vec3>&							_normals;
		const std::vector<vec2>&							_uvs;
		std::unordered_map<std::string, std::vector<Face>>	_faces;
		Mesh 												_mesh;

		void addTriangle(VertexIndex a, VertexIndex b, VertexIndex c, vec3 color);
		void convertToGpuData();
};

#endif