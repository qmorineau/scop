#include "MeshBuilder.hpp"

MeshBuilder::MeshBuilder(const std::vector<vec3>& p,
	const std::unordered_map<vec3, int,
		vec3::Vec3Hash, vec3::Vec3Eq>& pidx,
	const std::vector<vec3>& n,
	const std::unordered_map<vec3, int,
		vec3::Vec3Hash, vec3::Vec3Eq>& nidx,
	const std::vector<vec2>& uv,
	const std::unordered_map<vec2, int,
		vec2::Vec2Hash, vec2::Vec2Eq>& uvidx,
	size_t facesNbr) :
		_positions(p),
		_posIndices(pidx),
		_normals(n),
		_normIndices(nidx),
		_uvs(uv),
		_uvsIndices(uvidx)
{
	_faces.reserve(facesNbr);
	_mesh.vertices.reserve(facesNbr * 3);
	_allNormals = n;
	_allUvs = _uvs;
};

Mesh MeshBuilder::build(std::unordered_map<std::string, Material>& materials)
{
	if (_faces.empty())
		throw EmptyMesh(".obj file never define any face");
	std::cout << "Create Normals" << std::endl;
	createNormals();
	std::cout << "Create Smooth Normals" << std::endl;
	createSmoothNormals();
	std::cout << "Creating Uvs" << std::endl;
	createUvs();
	std::cout << "Convert to Gpu Data" << std::endl;
	convertToGpuData(materials);
	return _mesh;
}

void MeshBuilder::convertToGpuData(std::unordered_map<std::string, Material>& materials)
{
	for (auto& [materialName, faces] : _faces)
	{
		Material& mat = materials.at(materialName);
		size_t i = 0;
		for (auto& face : faces)
		{
			std::vector<VertexIndex> &v = face.vertices;
			for (unsigned int j = 1; j < v.size() - 1; j++)
				addTriangle(v[0], v[j + 1], v[j], mat);
			i++;
		}
	}
}

void MeshBuilder::addTriangle(const VertexIndex& a, const VertexIndex& b, const VertexIndex& c, Material& mat)
{
	const VertexIndex index[3] = {a, b, c};	
	float rand = (float) std::rand() / (float)RAND_MAX;

	for (int i = 0; i < 3; i++)
	{
		const vec3& pos = _positions[index[i].vertex];
		const vec3& norm = _allNormals[index[i].normal];
		const vec2& uv = _allUvs[index[i].uv];
		Vertex v = Vertex(pos, norm, uv, vec3(rand, rand, rand));
		_mesh.addVertex(v, &mat);
	}
}

void MeshBuilder::addFace(Face& f, Material& m)
{
	auto [it, isNew] = _faces.try_emplace(m._name);
	it->second.push_back(f);
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
			if (face.hasNormal)
				continue;

			const vec3& p0 = _positions[face.vertices[0].vertex];
			const vec3& p1 = _positions[face.vertices[1].vertex];
			const vec3& p2 = _positions[face.vertices[2].vertex];

			vec3 n = math::normalize(math::cross(p1 - p0, p2 - p0));

			int id = -1;
			auto it = _normIndices.find(n);
			if (it != _normIndices.end()) {
				id = it->second;
			} else {
				id = _allNormals.size();
				_allNormals.push_back(n);
				_normIndices[n] = id;
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

void MeshBuilder::accumulateFaceNormals(std::unordered_map<int, vec3>& accum, std::unordered_map<int, int>&  count)
{
	for (auto& [material, faces] : _faces)
    {
        for (Face& face : faces)
        {
			if (face.smoothingGroup != 0)
			{
				const vec3& p0 = _positions[face.vertices[0].vertex];
				const vec3& p1 = _positions[face.vertices[1].vertex];
				const vec3& p2 = _positions[face.vertices[2].vertex];

				vec3 n = math::normalize(math::cross(p2 - p0, p1 - p0));

				for (auto& v : face.vertices)
				{
					accum[v.vertex] += n;
					count[v.vertex]++;
				}
			}
        }
    }
}

void MeshBuilder::dedupeNormals(std::unordered_map<int, vec3>& accum, std::unordered_map<int, int>&  count)
{
	int id;
	vec3 smooth;

	for (auto& [material, faces] : _faces)
    {
        for (Face& face : faces)
        {
			if (face.smoothingGroup != 0)
			{
				for (auto& v : face.vertices)
				{
					smooth = math::normalize(accum[v.vertex]) / float(count[v.vertex]);
					auto it = _normIndices.find(smooth);
					if (it != _normIndices.end())
						id = it->second;
					else
					{
						id = static_cast<int>(_allNormals.size());
						_allNormals.push_back(smooth);
						_normIndices[smooth] = id;
					}
					v.normal = id;
				}
			}
        }
    }
}

void MeshBuilder::createSmoothNormals()
{
    std::unordered_map<int, vec3> accum;
    std::unordered_map<int, int>  count;

	accumulateFaceNormals(accum, count);
	dedupeNormals(accum, count);
}

void MeshBuilder::createUvs()
{
    _allUvs.clear();

    for (auto& [material, faces] : _faces)
    {
        for (Face& face : faces)
        {
			if (face.hasTextCoord)
				continue;
            for (auto& v : face.vertices)
            {
                const vec3& p = _positions[v.vertex];

                // Spherical projection
                float u = 0.5f + atan2(p.z, p.x) / (2.0f * M_PI);
                float vcoord = 0.5f - asin(p.y / std::sqrt(p.x * p.x + p.y * p.y + p.z * p.z)) / M_PI;

                vec2 uv(u, vcoord);

                // Store and index
                _allUvs.push_back(uv);
                v.uv = static_cast<int>(_allUvs.size() - 1);
            }
        }
    }
}
