#ifndef MESHBUILDER_HPP
#define MESHBUILDER_HPP

#include <vector>
#include <unordered_map>

#include "Math.hpp"
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
			Face(int obj, int mat, std::vector<int> groups, int smooth) : 
				objectIndex(obj),
				materialIndex(mat),
				smoothingGroup(smooth),
				groupIndices(groups) {};

			int							objectIndex;
			int							materialIndex;
			int							smoothingGroup;
			std::vector<int>			groupIndices;
			bool						hasTextCoord = false;
			bool						hasNormal = false;
			std::vector<VertexIndex> 	vertices;
		};

		MeshBuilder(const std::vector<vec3>& p,
			const std::unordered_map<vec3, int,
				vec3::Vec3Hash, vec3::Vec3Eq>& pidx,
			const std::vector<vec3>& n,
			const std::unordered_map<vec3, int,
				vec3::Vec3Hash, vec3::Vec3Eq>& nidx,
			const std::vector<vec2>& uv,
			const std::unordered_map<vec2, int,
				vec2::Vec2Hash, vec2::Vec2Eq>& uvidx,
			size_t facesNbr);

		Mesh build(std::unordered_map<std::string, Material>&);
		void addFace(Face& f, Material& m);

	private:
		const std::vector<vec3>&							_positions;
		std::unordered_map<vec3, int,
			vec3::Vec3Hash, vec3::Vec3Eq>					_posIndices;
		const std::vector<vec3>&							_normals;
		std::unordered_map<vec3, int,
			vec3::Vec3Hash, vec3::Vec3Eq>					_normIndices;
		const std::vector<vec2>&							_uvs;
		std::unordered_map<vec2, int,
			vec2::Vec2Hash, vec2::Vec2Eq>					_uvsIndices;
		std::vector<vec3>									_allNormals;
		std::vector<vec2>									_allUvs;
		std::unordered_map<std::string, std::vector<Face>>	_faces;
		Mesh							 					_mesh;

		void	addTriangle(const VertexIndex&, const VertexIndex&, const VertexIndex&, Material&);
		void	convertToGpuData(std::unordered_map<std::string, Material>&);

		int		findDuplicateNormal(vec3& v);
		void	createNormals();
		void	createSmoothNormals();
		void 	accumulateFaceNormals(std::unordered_map<int, vec3>&, std::unordered_map<int, int>&);
		void 	dedupeNormals(std::unordered_map<int, vec3>&,  std::unordered_map<int, int>&);
		void	createUvs();

};

#endif