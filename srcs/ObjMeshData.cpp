#include "ObjMeshData.hpp"
#include "Vertex.hpp"

ObjMeshData::ObjMeshData() {}

ObjMeshData::ObjMeshData(const ObjMeshData& copy) {*this = copy;}

ObjMeshData::ObjMeshData(const std::string& file) : _filePath(file) {}

ObjMeshData& ObjMeshData::operator=(const ObjMeshData& other)
{
	if (this != &other)
	{
		_filePath = other._filePath;
		_vertices = other._vertices;
	}
	return (*this);
}

ObjMeshData::~ObjMeshData() 
{

}

void ObjMeshData::parsePosition(std::istringstream& iss)
{
	float x = 0;
	float y = 0;
	float z = 0;

	iss >> x;
	iss >> y;
	iss >> z;

	_positions.push_back(vec3(x, y, z));
}

void ObjMeshData::parseVertexNormal(std::istringstream& iss)
{
	float x = 0;
	float y = 0;
	float z = 0;

	iss >> x;
	iss >> y;
	iss >> z;

	vec3 v = math::normalize(vec3(x, y, z));
	_normals.push_back(v);
}

void ObjMeshData::parseTexture(std::istringstream& iss)
{
	float x = 0;
	float y = 0;

	iss >> x;
	iss >> y;

	Vector2 v(x, y);
	_textures.push_back(v);
}

int ObjMeshData::findDuplicateVertex(Vertex& v)
{
	for (unsigned int i = 0; i < _vertices.size(); i++)
	{
		if (v == _vertices[i])
			return (i);
	}
	return (0);
}

int ObjMeshData::findDuplicateNormal(vec3& v)
{
	for (unsigned int i = 0; i < _normals.size(); i++)
	{
		if (v == _normals[i])
			return (i);
	}
	return (0);
}

int ObjMeshData::findDuplicatePosition(vec3& v)
{
	for (unsigned int i = 0; i < _positions.size(); i++)
	{
		if (v == _positions[i])
			return (i);
	}
	return (0);
}

ObjMeshData::VertexIndex ObjMeshData::parseVertex(const std::string& indices)
{
	std::stringstream ss(indices);
	std::string vStr, vtStr, vnStr;

	std::getline(ss, vStr, '/');
	std::getline(ss, vtStr, '/');
	std::getline(ss, vnStr, '/');

	VertexIndex index;
	index.vertex = vStr.empty() ? 0 : std::stoi(vStr.c_str());
	index.textCoord = vtStr.empty() ? 0 : std::stoi(vtStr.c_str());
	index.normal = vnStr.empty() ? 0 : std::stoi(vnStr.c_str());
	
	return (index);
}

void ObjMeshData::createFace(std::istringstream& iss)
{
	std::string word;
	Face face;

	while (iss >> word)
		face.vertices.push_back(parseVertex(word));
	_faces.push_back(face);
}

void ObjMeshData::createNormal()
{
	for (unsigned int i = 0; i < _faces.size(); i++)
	{
		if (_faces[i].vertices[0].normal)
			continue;
		vec3 a = _positions[_faces[i].vertices[1].vertex] - _positions[_faces[i].vertices[0].vertex];
		vec3 b = _positions[_faces[i].vertices[2].vertex] - _positions[_faces[i].vertices[0].vertex];
		vec3 normal = math::cross(a, b);
		normal = math::normalize(normal);
		int id = findDuplicateNormal(normal);
		for (auto& v : _faces[i].vertices)
			v.normal = !id ? _normals.size() + 1 : id + 1;
		if (!id)
			_normals.push_back(normal);
	}
}

void ObjMeshData::createTextCoord()
{
	for (unsigned int i = 0; i < _faces.size(); i++)
	{
		// if (_faces[i].vertices[0].textCoord)
		// 	continue;

		Face face = _faces[i];
		vec2 textCoord;

		for (auto& v : face.vertices)
		{
			float mx = std::abs(_normals[v.normal].x);
			float my = std::abs(_normals[v.normal].y);
			float mz = std::abs(_normals[v.normal].z);

			if (mx >= my && mx >= mz)
				textCoord = vec2(mz * 5, my * 5);
			if (my >= mz && my >= mx)
				textCoord = vec2(mx * 5 , mz* 5);
			else
				textCoord = vec2(mx* 5, my* 5);

			for (auto& t : face.vertices)
				t.textCoord = _textures.size();
			_textures.push_back(textCoord);
		}
	}
}

void ObjMeshData::addTriangle(VertexIndex a, VertexIndex b, VertexIndex c)
{
	VertexIndex index[] = {a, b, c};
	Vertex vertex[3];
	for (int i = 0; i < 3; i++)
	{
		vertex[i] = Vertex(_positions[index[i].vertex - 1], _normals[index[i].normal - 1], _textures[index[i].textCoord]);
		int id = findDuplicateVertex(vertex[i]);
		if (id)
			_indices.push_back(id);
		else
		{
			_indices.push_back(_vertices.size());
			_vertices.push_back(vertex[i]);
		}
	}
}

void ObjMeshData::convertToGpuData()
{
	for (unsigned int i = 0; i < _faces.size(); i++)
	{
		std::vector<ObjMeshData::VertexIndex> &v = _faces[i].vertices;
		for (unsigned int j = 1; j < v.size() - 1; j++)
			addTriangle(v[0], v[j], v[j + 1]);
	}
}

void ObjMeshData::centerMesh()
{
	std::cout << _positions.size() << std::endl;
	vec3 min(_positions[0]);
	vec3 max(_positions[0]);

	for (auto& v : _positions)
	{
		if (v.x < min.x)
			min.x = v.x;
		if (v.x > max.x)
			max.x = v.x;
		if (v.y < min.y)
			min.y = v.y;
		if (v.y > max.y)
			max.y = v.y;
		if (v.z < min.z)
			min.z = v.z;
		if (v.z > max.z)
			max.z = v.z;
	}

	vec3 center((min + max) * 0.5f);
	vec3 size = max - min;

	float maxExtent = std::max(size.x, std::max(size.y, size.z));
	float scale = 2.0f / maxExtent;

	for (auto& v : _positions)
	{
		v -= center;
		v *= scale;
	}
}

void ObjMeshData::parse()
{
	std::ifstream inFile(_filePath);

	if (!inFile.is_open())
	{
		// throw error
	}
	std::string line;
	std::string token;
	while (std::getline(inFile, line))
	{
		std::istringstream iss(line);
		if (iss >> token)
		{
			if (token.c_str()[0] == '#')
				continue;
			else if (!std::strcmp("v", token.c_str()))
				parsePosition(iss);
			else if (!std::strcmp("vt", token.c_str()))
				parseTexture(iss);
			else if (!std::strcmp("vn", token.c_str()))
				parseVertexNormal(iss);
			else if (!std::strcmp("vp", token.c_str()))
				continue;
			else if (!std::strcmp("f", token.c_str()))
				createFace(iss);
			else
				continue; // wrong token, error ?
		}
	}
	centerMesh();
	createNormal();
	createTextCoord();
	convertToGpuData();
}

void ObjMeshData::print()
{
	for	(unsigned int i = 0; i < _indices.size(); i++)
	{
		std::cout << "ID " << i + 1 << " = index " << _indices[i] << " = ";
		_vertices[_indices[i]].print();
	}
}