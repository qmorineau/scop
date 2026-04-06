#include "ObjParser.hpp"

// Constructor
ObjParser::ObjParser(std::string file) :
	_pathFile(file),
	_file(file),
	_actualMaterial("__default_42scop_material")
{
	_materials.try_emplace("__default_42scop_material", Material(_actualMaterial));
	if (!_file.is_open())
		throw ParseError("ObjParser: Can't open \"" + file + "\"");
	parse();
	centerPositions();
	normalizePositions();
	build();
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
	{"s", &ObjParser::parseSmoothing}, // on/off smoothing, flat rendering or not
	// {"g", &ObjParser::} // group faces togeter (wheel, door etc.. for a car)
	{"l", &ObjParser::createLine}
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
}

void ObjParser::build()
{
	for (auto& object : _rawData)
	{
		MeshBuilder builder(_positions, _normals, _uvs, object.name);
		for (auto& [material, faces] : object.faces)
		{
			for (auto& face : faces)
				builder.addFace(face, _materials.at(material));
		}
		_meshes.push_back(builder.build(_materials));
	}
};

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
	ObjectData newObj;
	newObj.name = name;
	_rawData.push_back(newObj);
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
	index.uv = vtStr.empty() ? -1 : std::stoi(vtStr.c_str()) - 1;
	index.normal = vnStr.empty() ? -1 : std::stoi(vnStr.c_str()) - 1;
	return (index);
}

void ObjParser::createFace(std::istringstream& iss)
{
	std::string word;
	MeshBuilder::Face face;

	while (iss >> word)
		face.vertices.push_back(parseVertex(word));
	if (!_rawData.size())
		_rawData.push_back(ObjectData());
	if (!_rawData.at(_rawData.size() - 1).faces.size())
	{
		std::vector<MeshBuilder::Face> faces;
		_rawData.at(_rawData.size() - 1).faces.try_emplace(_actualMaterial, faces);
	}
	try
	{
		_rawData.at(_rawData.size() - 1).faces.at(_actualMaterial);
	}
	catch(const std::exception& e)
	{
		std::vector<MeshBuilder::Face> faces;
		_rawData.at(_rawData.size() - 1).faces.try_emplace(_actualMaterial, faces);
	}
	_rawData.at(_rawData.size() - 1).faces.at(_actualMaterial).push_back(face);
};

void ObjParser::createLine(std::istringstream& iss)
{
	(void) iss;
	/* std::string word;
	MeshBuilder::Face face;

	while (iss >> word)
		face.vertices.push_back(parseVertex(word));
	if (!_rawData.size())
		_rawData.push_back(ObjectData());
	if (!_rawData.at(_rawData.size() - 1).faces.size())
	{
		std::vector<MeshBuilder::Face> faces;
		_rawData.at(_rawData.size() - 1).faces.try_emplace(_actualMaterial, faces);
	}
	_rawData.at(_rawData.size() - 1).faces.at(_actualMaterial).push_back(face); */
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
		for (auto& m : map)
			_materials.try_emplace(m.first, m.second);
	}
}

void ObjParser::parseSmoothing(std::istringstream& iss)
{
	std::string value;
	if (!(iss >> value))
		throw ParseError("ObjParser: 's' expect 1 string value");
	std::string extra;
	if (iss >> extra)
		throw ParseError("ObjParser: Unexpected extra value: " + extra);

	if (value != "off" && value != "1")
		throw ParseError("ObjParser: 's' expect 'on' / 'off' value");
	// do something from on or off
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

void ObjParser::centerPositions()
{
	vec3 objMin(FLT_MAX);
	vec3 objMax(-FLT_MAX);

	for (auto v : _positions)
	{
		objMin = vec3::min(objMin, v);
		objMax = vec3::max(objMax, v);
	}

	vec3 offset((objMin + objMax) * 0.5f);
	for (auto& v : _positions)
		v -= offset;
}

void ObjParser::normalizePositions()
{
	float maxR2 = 0.f;

	for (const auto& v : _positions)
		maxR2 = std::max(maxR2, v.lengthSquared());

	float maxRadius = std::sqrt(maxR2);

	float scale = 1.0f / maxRadius;

	for (auto& v : _positions)
		v *= scale;
}

void ObjParser::printRawData()
{
	for (auto& object : _rawData)
	{
		std::cerr << "Object = " << object.name << std::endl;
		for (auto& [material, faces] : object.faces)
		{
			std::cerr << "Material = " << material << std::endl;
			for (auto& face : faces)
			{
				std::cerr << "has normal = " << face.hasNormal << ", has texture = " << face.hasTextCoord << std::endl;
				for (auto& v : face.vertices)
					std::cerr << v.vertex << "/" << v.normal << "/" << v.uv << std::endl; 
			}
		}
	}
}