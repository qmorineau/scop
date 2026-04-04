#include "MeshBuilder.hpp"

MeshBuilder::MeshBuilder(const std::vector<vec3>& p, const std::vector<vec3>& n, const std::vector<vec2>& uv, std::string name) :
	_positions(p),
	_normals(n),
	_uvs(uv),
	_mesh(name)
{
	_allNormals = n;
	_allUvs = uv;
};

Mesh MeshBuilder::build()
{
	if (_faces.empty())
		throw EmptyMesh("");
	createNormals();
	createUvs();
	convertToGpuData();
	return _mesh;
}

void MeshBuilder::addTriangle(VertexIndex a, VertexIndex b, VertexIndex c, vec3 color)
{
	VertexIndex index[] = {a, b, c};

	for (int i = 0; i < 3; i++)
	{
		const vec3& pos = _positions[index[i].vertex];
		const vec3& norm = _allNormals[index[i].normal];
		const vec2& uv = _allUvs[index[i].textCoord];
		Vertex v = Vertex(pos, norm, uv, color);
		_mesh.addVertex(v);
	}
}

void MeshBuilder::convertToGpuData()
{
	for (auto it : _faces)
	{
		size_t i = 0;
		for (auto face : it.second)
		{
			std::vector<VertexIndex> &v = face.vertices;
			for (unsigned int j = 1; j < v.size() - 1; j++)
				addTriangle(v[0], v[j], v[j + 1], faceColors[i % 4]); // choose good mesh
			i++;
		}
	}
}

void MeshBuilder::addFace(Face& f, Material& m)
{
	_faces[m._name].push_back(f);
}

int MeshBuilder::findDuplicateNormal(vec3& v)
{
	for (unsigned int i = 0; i < _normals.size(); i++)
	{
		if (v == _normals[i])
			return (i);
	}
	return (-1);
}

void MeshBuilder::createNormals()
{
	for (auto& [material, faces] : _faces)
	{
		for (Face& face : faces)
		{
			if (!face.vertices.empty() && face.vertices[0].normal != -1)
				continue;

			const vec3& p0 = _positions[face.vertices[0].vertex];
			const vec3& p1 = _positions[face.vertices[1].vertex];
			const vec3& p2 = _positions[face.vertices[2].vertex];

			vec3 n = math::normalize(math::cross(p1 - p0, p2 - p0));

			int id = -1;
			for (size_t i = 0; i < _allNormals.size(); i++)
			{
				if (_allNormals[i] == n)
					id = static_cast<int>(i);
			}
			if (id == -1)
			{
				id = static_cast<int>(_allNormals.size());
				_allNormals.push_back(n);
			}

			for (auto& v : face.vertices)
				v.normal = id;
		}
	}
}

void MeshBuilder::createUvs()
{
	for (auto& [material, faces] : _faces)
	{
		for (Face& face : faces)
		{
			if (!face.vertices.empty() && face.vertices[0].textCoord != -1)
				continue;

			for (auto& v : face.vertices)
			{
				vec2 uv;
				float mx = std::abs(_allNormals[v.normal].x);
				float my = std::abs(_allNormals[v.normal].y);
				float mz = std::abs(_allNormals[v.normal].z);

				if (mx >= my && mx >= mz)
					uv = vec2(mz * 5, my * 5);
				if (my >= mz && my >= mx)
					uv = vec2(mx * 5 , mz* 5);
				else
					uv = vec2(mx* 5, my* 5);

				for (auto& t : face.vertices)
					t.textCoord = _uvs.size();

				_allUvs.push_back(uv);
			}
		}
	}
}
