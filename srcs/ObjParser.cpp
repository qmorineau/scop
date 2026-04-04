#include "ObjParser.hpp"

// Constructor
ObjParser::ObjParser(std::string file) :
	_pathFile(file),
	_file(file),
	_actualMaterial("__default_42scop_material")
{
	_builder = std::make_unique<MeshBuilder>(_positions, _normals, _uvs, "__default_42scop_mesh");
	if (!_file.is_open())
		throw ParseError("ObjParser: Can't open \"" + file + "\"");
	_materials.try_emplace(_actualMaterial, Material(_actualMaterial));
	parse();
};

// Dispatch Table
const std::unordered_map<std::string, ObjParser::Handler> ObjParser::handlers =
{
	{"v", &ObjParser::parsePosition},
	{"vt", &ObjParser::parseUvs},
	{"vn", &ObjParser::parseVertexNormal},
	// {"vp", &ObjParser::}, // ne les fait pas, complex courbe etc...
	{"f", &ObjParser::createFace},
	{"mtllib", &ObjParser::parseMtlFile},
	{"o", &ObjParser::parseObjectName}, // object name
	{"usemtl", &ObjParser::useMtl}, // use material for the face that comes
	// {"s", &ObjParser::}, // on/off smoothing, flat rendering or not
	// {"g", &ObjParser::} // group faces togeter (wheel, door etc.. for a car)
};

// Parser
void ObjParser::parse()
{
	std::string line;
	std::string token;
	while (std::getline(_file, line))
	{
		std::istringstream iss(line);
		if (iss >> token)
		{
			if (token.empty() || token.c_str()[0] == '#')
				continue;

			auto iterator = handlers.find(token);
			if (iterator == handlers.end())
				throw ParseError("ObjParser: Unknown token: " + token);
			Handler h = iterator->second;
			(this->*h)(iss);
		}
	}
	_builder->build();
}

void ObjParser::parsePosition(std::istringstream& iss)
{
	vec3 v;
	if (!(iss >> v.x >> v.y >> v.z))
		throw ParseError("ObjParser: 'v' expect 3 floats values");
	std::string extra;
	if (iss >> extra)
		throw ParseError("ObjParser: Unexpected extra value: " + extra);
	_positions.push_back(v);
};

void ObjParser::parseVertexNormal(std::istringstream& iss)
{
	vec3 v;
	if (!(iss >> v.x >> v.y >> v.z))
		throw ParseError("ObjParser: 'vn' expect 3 floats values");
	std::string extra;
	if (iss >> extra)
		throw ParseError("ObjParser: Unexpected extra value: " + extra);
	_normals.push_back(v);
};

void ObjParser::parseUvs(std::istringstream& iss)
{
	vec2 v;
	if (!(iss >> v.x >> v.y))
		throw ParseError("ObjParser: 'vt' expect 2 floats values");
	std::string extra;
	if (iss >> extra)
		throw ParseError("ObjParser: Unexpected extra value: " + extra);
	_uvs.push_back(v);
};

void ObjParser::parseObjectName(std::istringstream& iss)
{
	std::string name;
	if (!(iss >> name))
		throw ParseError("ObjParser: 'o' expect 1 string value");
	std::string extra;
	if (iss >> extra)
		throw ParseError("ObjParser: Unexpected extra value: " + extra);
	try
	{
		_meshes.push_back(_builder->build());
	}
	catch(const std::exception& e)
	{
		std::cerr << "ObjParser: " << e.what() << std::endl;
	}
	_builder = std::make_unique<MeshBuilder>(_positions, _normals, _uvs, name);
}

MeshBuilder::VertexIndex ObjParser::parseVertex(const std::string& indices)
{
	std::stringstream ss(indices);
	std::string vStr, vtStr, vnStr;

	std::getline(ss, vStr, '/');
	std::getline(ss, vtStr, '/');
	std::getline(ss, vnStr, '/');

	MeshBuilder::VertexIndex index;
	index.vertex = vStr.empty() ? -1 : std::stoi(vStr.c_str()) - 1;
	index.textCoord = vtStr.empty() ? -1 : std::stoi(vtStr.c_str()) - 1;
	index.normal = vnStr.empty() ? -1 : std::stoi(vnStr.c_str()) - 1;
	return (index);
}

void ObjParser::createFace(std::istringstream& iss)
{
	std::string word;
	MeshBuilder::Face face;

	while (iss >> word)
		face.vertices.push_back(parseVertex(word));
	_builder->addFace(face, _materials.at(_actualMaterial));
};

void ObjParser::parseMtlFile(std::istringstream& iss)
{
	std::string path = _pathFile;
	path = path.substr(0, path.find_last_of('/') + 1); // safe ????

	std::string word;
	while (iss >> word)
	{
		MtlParser parser(path, word);
		auto map = parser.parse();
		for (auto m : map)
			_materials.try_emplace(m.first, m.second);
	}
}

void ObjParser::useMtl(std::istringstream& iss)
{

	std::string word;
	if (!(iss >> word))
		throw ParseError("ObjParser: 'usemtl' expect 1 string value");
	std::string extra;
	if (iss >> extra)
		throw ParseError("ObjParser: Unexpected extra value: " + extra);
	_actualMaterial = word;
}

void ObjParser::centerMeshes()
{
	vec3 objMin(FLT_MAX);
	vec3 objMax(-FLT_MAX);

	for (auto v : _positions)
	{
		objMin = vec3::min(objMin, v);
		objMax = vec3::max(objMax, v);
	}

	vec3 offset((objMin + objMax) * 0.5f);
	for (auto v : _positions)
		v += offset;
}
