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

	vec3 v = math::normalize(vec3(x, y, z));
	_positions.push_back(v);
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

	for (int i = 0; i < 3; i++)
	{
		iss >> word;
		face.vertices[i] = parseVertex(word);
	}
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
		_faces[i].vertices[0].normal = _faces[i].vertices[1].normal = _faces[i].vertices[2].normal = !id ? _normals.size() + 1 : id + 1;
		if (!id)
			_normals.push_back(normal);
	}
}

void ObjMeshData::createTextCoord()
{
	for (unsigned int i = 0; i < _faces.size(); i++)
	{
		if (_faces[i].vertices[0].textCoord)
			continue;
		vec2 textCoord; // to remove

		_faces[i].vertices[0].textCoord = _faces[i].vertices[1].textCoord = _faces[i].vertices[2].textCoord = _textures.size() + 1; // to change
		_textures.push_back(textCoord); // to change
	}
}

void ObjMeshData::convertToGpuData()
{
	for (unsigned int i = 0; i < _faces.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			// std::cout << _positions.size() << " " << _normals.size() << " " << _textures.size() << std::endl;
			// std::cout << _faces[i].vertices[j].vertex - 1 << " " << _faces[i].vertices[j].normal - 1 << " " << _faces[i].vertices[j].textCoord - 1 << std::endl;
			Vertex vertex(_positions[_faces[i].vertices[j].vertex - 1],
							_normals[_faces[i].vertices[j].normal - 1],
							_textures[_faces[i].vertices[j].textCoord - 1]); // should be parsed and add -1
			int id = findDuplicateVertex(vertex);
			if (id)
				_indices.push_back(id);
			else
			{
				_indices.push_back(_vertices.size());
				_vertices.push_back(vertex);
			}			
		}
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