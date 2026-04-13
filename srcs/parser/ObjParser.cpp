#include "ObjParser.hpp"

// Helpers

static int getIndex(const std::string& name,
	std::vector<std::string>& names,
	std::unordered_map<std::string, int>& map)
{
    auto it = map.find(name);
    if (it != map.end())
        return it->second;

    int index = names.size();
    names.push_back(name);
    map[name] = index;
    return index;
}

static bool toInt(const std::string& s, int& out) {
    std::stringstream ss(s);
    ss >> out;
    return !ss.fail() && ss.eof();
}

// Constructor
ObjParser::ObjParser(std::string file) :
	_pathFile(file),
	_pathFolder(file),
	_file(file)
{
	size_t pos = _pathFolder.find_last_of("/\\");
	if (pos == std::string::npos)
		_pathFolder = "";
	else
		_pathFolder = _pathFolder.substr(0, pos + 1);

	std::string defaultMaterialName("__default_42scop_material");
	_materials.try_emplace(defaultMaterialName, Material(defaultMaterialName));
	materialNames.push_back(defaultMaterialName);

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
	{"v", &ObjParser::parsePosition},				// new vertex
	{"vt", &ObjParser::parseUvs},					// new Uvs / Vertex Texture
	{"vn", &ObjParser::parseVertexNormal}, 			// new Vertex Normal
	{"vp", &ObjParser::parseParameterSpaceVertex},	// New Space Vertex Parameter
	{"f", &ObjParser::createFace},					// new Face
	{"mtllib", &ObjParser::parseMtlFile}, 			// new Material File
	{"o", &ObjParser::parseObjectName}, 			// change Object Name State
	{"usemtl", &ObjParser::useMtl}, 				// change Material State
	{"s", &ObjParser::parseSmoothing}, 				// define Smoothing Group (average normal for each faces)
	{"g", &ObjParser::parseGroupName}, 				// change Group Name State
	{"l", &ObjParser::createLine} 					// define new Lines
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
	std::cout << "Parsing of '"<<_pathFile << "' is done" << std::endl;
}

void ObjParser::build()
{
	size_t facesNumber = 0;
	for (auto& [material, faces] : _rawData.faces)
	{
		facesNumber += faces.size();
	}
	std::cout << "Number of vertices: " << _positions.size() << std::endl;
	std::cout << "Number of faces: " << facesNumber << std::endl;
	MeshBuilder builder(_positions, _positionsIdx, _normals, _normalsIdx, _uvs, _uvsIdx, facesNumber);
	for (auto& [material, faces] : _rawData.faces)
	{
		for (auto& face : faces)
			builder.addFace(face, _materials.at(material));
	}
	_mesh = builder.build(_materials);
	std::cout << "Mesh Builded" << std::endl;
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
	idxActualObject = getIndex(name, objectNames, objectIndices);
}

void ObjParser::parseGroupName(std::istringstream& iss)
{
	std::string name;
	idxActualGroups.clear();
	while (iss >> name)
		idxActualGroups.push_back(getIndex(name, groupNames, groupIndices));
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

void ObjParser::parseParameterSpaceVertex(std::istringstream& iss)
{
	(void) iss;
	std::cerr << "ObjParser: 'vp' key not yet supported" << std::endl;
}

void ObjParser::createFace(std::istringstream& iss)
{
	std::string word;
	MeshBuilder::Face face(idxActualObject, idxActualMaterial, idxActualGroups, actualSmoothingGroup);

	while (iss >> word)
	{
		face.vertices.push_back(parseVertex(word));
		int& normal = face.vertices[face.vertices.size() - 1].normal;
		int& uvs = face.vertices[face.vertices.size() - 1].uv;
		if (normal != -1)
			face.hasNormal = true;
		if (uvs != -1)
			face.hasTextCoord = true;
	}
	std::string materialName = materialNames[idxActualMaterial];
	if (!_rawData.faces.size())
	{
		std::vector<MeshBuilder::Face> faces;
		_rawData.faces.try_emplace(materialName, faces);
	}
	try
	{
		_rawData.faces.at(materialName);
	}
	catch(const std::exception& e)
	{
		std::vector<MeshBuilder::Face> faces;
		_rawData.faces.try_emplace(materialName, faces);
	}
	_rawData.faces.at(materialName).push_back(face);
};

void ObjParser::createLine(std::istringstream& iss)
{
	(void) iss;
};

void ObjParser::parseMtlFile(std::istringstream& iss)
{
	std::string word;
	while (iss >> word)
	{
		MtlParser parser(_pathFolder, word);
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
	if (value != "off")
	{
		int intValue;
		if (!toInt(value, intValue))
			throw ParseError("ObjParser: 's' expect 'off' or 'int value' ");
		if (intValue < 0)
			throw ParseError("ObjParser: 's' expect positive int value");
		actualSmoothingGroup = intValue;
	}
	else
		actualSmoothingGroup = 0;
}


void ObjParser::useMtl(std::istringstream& iss)
{

	std::string word;
	if (!(iss >> word))
		throw ParseError("ObjParser: 'usemtl' expect 1 string value");
	std::string extra;
	if (iss >> extra)
		throw ParseError("ObjParser: Unexpected extra value: " + extra);
	idxActualMaterial = getIndex(word, materialNames, materialIndices);
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
	for (auto& [material, faces] : _rawData.faces)
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