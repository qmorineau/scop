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
	_allUvs = uv;
};

Mesh MeshBuilder::build(const std::unordered_map<std::string, Material>& materials)
{
	std::cerr << "Mesh Builder : Build" << std::endl;
	// if (_faces.empty())
	// 	throw EmptyMesh(""); // ICI to readd after adding the line of the model
	// if (smooth)

	// else
	createNormals();
	createUvs();
	convertToGpuData(materials);
	return _mesh;
}

void MeshBuilder::convertToGpuData(const std::unordered_map<std::string, Material>& materials)
{
	std::cerr << "Begin to convert to Gpu Data" << std::endl;
	for (auto& [materialName, faces] : _faces)
	{
		const Material& mat = materials.at(materialName);
		size_t i = 0;
		for (auto& face : faces)
		{
			std::vector<VertexIndex> &v = face.vertices;
			for (unsigned int j = 1; j < v.size() - 1; j++)
				addTriangle(v[0], v[j + 1], v[j], mat); // choose good mesh
			i++;
		}
	}
	std::cerr << "Finish to convert to Gpu Data" << std::endl;
}

void MeshBuilder::addTriangle(const VertexIndex& a, const VertexIndex& b, const VertexIndex& c, const Material& mat)
{
	const VertexIndex index[3] = {a, b, c};
	
	float rand = (float) std::rand() / RAND_MAX;
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
	std::cerr << "Create Normals" << std::endl;
	for (auto& [material, faces] : _faces)
	{
		for (Face& face : faces)
		{
			// if (!face.vertices.empty() && face.vertices[0].normal != -1)
			// 	continue;

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

void MeshBuilder::createSmoothNormals()
{
    std::cerr << "Create Smooth Normals" << std::endl;

    // Accumulate normals per vertex index
    std::unordered_map<int, vec3> accum;
    std::unordered_map<int, int>  count;

    // 1. Accumulate face normals
    for (auto& [material, faces] : _faces)
    {
        for (Face& face : faces)
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

    // 2. Normalize and dedupe normals using the hash table
    for (auto& [material, faces] : _faces)
    {
        for (Face& face : faces)
        {
            for (auto& v : face.vertices)
            {
                vec3 smooth = math::normalize(accum[v.vertex]);

                // Fast O(1) lookup
                auto it = _normIndices.find(smooth);
                int id;

                if (it != _normIndices.end())
                {
                    id = it->second;
                }
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

// void MeshBuilder::createUvs()
// {
// 	for (auto& [material, faces] : _faces)
// 	{
// 		for (Face& face : faces)
// 		{
// 			if (!face.vertices.empty() && face.vertices[0].uv != -1)
// 				continue;

// 			for (auto& v : face.vertices)
// 			{
// 				vec2 uv;
// 				float mx = std::abs(_allNormals[v.normal].x);
// 				float my = std::abs(_allNormals[v.normal].y);
// 				float mz = std::abs(_allNormals[v.normal].z);

// 				if (mx >= my && mx >= mz)
// 					uv = vec2(mz * 5, my * 5);
// 				else if (my >= mz && my >= mx)
// 					uv = vec2(mx * 5 , mz* 5);
// 				else
// 					uv = vec2(mx* 5, my* 5);

// 				_allUvs.push_back(uv);
// 				v.uv = static_cast<int>(_allUvs.size() - 1);
// 				std::cerr << _allUvs[v.uv] << std::endl;
// 			}
// 		}
// 	}
// }

void MeshBuilder::createUvs()
{
	std::cerr << "Create Uvs" << std::endl;
    _allUvs.clear();

    for (auto& [material, faces] : _faces)
    {
        for (Face& face : faces)
        {
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
