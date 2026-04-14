#include "MtlParser.hpp"

using Handler = void (MtlParser::*)(std::istringstream& iss);

// Dispath Table
static const std::unordered_map<std::string, Handler> handlers =
{
	{"Ns", &MtlParser::parseSpecularExponent},
	{"Ka", &MtlParser::parseAmbiantColor},
	{"Kd", &MtlParser::parseDiffuseColor},
	{"Ks", &MtlParser::parseSpecularColor},
	{"Ke", &MtlParser::parseEmissiveColor},
	{"Ni", &MtlParser::parseOpticalDensity},
	{"d", &MtlParser::parseTransparency},
	{"map_Kd", &MtlParser::parseTextureFile},
	{"map_d", &MtlParser::parseTextureFile},
	{"illum", &MtlParser::parseIlluminationModel}
};

std::unordered_map<std::string, Material> MtlParser::parse()
{
	std::cout << "Parsing Material File: " << _fileName << std::endl;
	std::string line;
	std::string token;
	while (std::getline(_file, line))
	{
		std::istringstream iss(line);
		if (iss >> token)
		{
			if (token.empty() || token.c_str()[0] == '#')
				continue;
			else if (token == "newmtl")
			{
				createNewMaterial(iss);
				continue;
			}

			auto iterator = handlers.find(token);
			if (iterator == handlers.end())
				throw ParseError("MtlParser: Unknown token: " + token);
			Handler h = iterator->second;
			(this->*h)(iss);
		}
	}
	return _materials;
}

void MtlParser::createNewMaterial(std::istringstream& iss)
{
	std::string name;
	if (!(iss >> name))
		throw ParseError("MtlParser: 'newmtl' expect 1 string values");
	std::string extra;
	if (iss >> extra)
		throw ParseError("MtlParser: Unexpected extra value: " + extra);
	_materials.try_emplace(name, Material(name));
	_actualMaterial = name;
};

void MtlParser::parseSpecularExponent(std::istringstream& iss)
{
	float f;
	if (!(iss >> f))
		throw ParseError("MtlParser: 'Ns' expect 1 floats values");
	std::string extra;
	if (iss >> extra)
		throw ParseError("MtlParser: Unexpected extra value: " + extra);
	_materials.at(_actualMaterial)._Ns = f;
};

void MtlParser::parseAmbiantColor(std::istringstream& iss)
{
	float r, g, b;
	if (!(iss >> r >> g >> b))
		throw ParseError("MtlParser: 'Ka' expect 3 floats values");
	std::string extra;
	if (iss >> extra)
		throw ParseError("MtlParser: Unexpected extra value: " + extra);
	if (r < 0.f || r > 1.f || g < 0.f || g > 1.f || b < 0.f || b > 1.f)
    	throw ParseError("'Ka' values must be between 0 and 1");
	_materials.at(_actualMaterial)._Ka = vec3(r, g, b);
};

void MtlParser::parseDiffuseColor(std::istringstream& iss)
{
	float r, g, b;
	if (!(iss >> r >> g >> b))
		throw ParseError("MtlParser: 'Kd' expect 3 floats values");
	std::string extra;
	if (iss >> extra)
		throw ParseError("MtlParser: Unexpected extra value: " + extra);
	if (r < 0.f || r > 1.f || g < 0.f || g > 1.f || b < 0.f || b > 1.f)
    	throw ParseError("'Kd' values must be between 0 and 1");
	_materials.at(_actualMaterial)._Kd = vec3(r, g, b);
};

void MtlParser::parseSpecularColor(std::istringstream& iss)
{
	float r, g, b;
	if (!(iss >> r >> g >> b))
		throw ParseError("MtlParser: 'Ks' expect 3 floats values");
	std::string extra;
	if (iss >> extra)
		throw ParseError("MtlParser: Unexpected extra value: " + extra);
	if (r < 0.f || r > 1.f || g < 0.f || g > 1.f || b < 0.f || b > 1.f)
    	throw ParseError("'Ks' values must be between 0 and 1");
	_materials.at(_actualMaterial)._Ks = vec3(r, g, b);
};

void MtlParser::parseEmissiveColor(std::istringstream& iss)
{
	float r, g, b;
	if (!(iss >> r >> g >> b))
		throw ParseError("MtlParser: 'Ke' expect 3 floats values");
	std::string extra;
	if (iss >> extra)
		throw ParseError("MtlParser: Unexpected extra value: " + extra);
	_materials.at(_actualMaterial)._Ke = vec3(r, g, b);
};

void MtlParser::parseOpticalDensity(std::istringstream& iss)
{
	float f;
	if (!(iss >> f))
		throw ParseError("MtlParser: 'Ni' expect 1 floats values");
	std::string extra;
	if (iss >> extra)
		throw ParseError("MtlParser: Unexpected extra value: " + extra);
	_materials.at(_actualMaterial)._Ni = f;
};

void MtlParser::parseTransparency(std::istringstream& iss)
{
	float f;
	if (!(iss >> f))
		throw ParseError("MtlParser: 'd' expect 1 floats values");
	std::string extra;
	if (iss >> extra)
		throw ParseError("MtlParser: Unexpected extra value: " + extra);
	_materials.at(_actualMaterial)._Ni = f;
};

static inline void trim(std::string& s)
{
    const char* ws = " \t\n\r\f\v";

    size_t start = s.find_first_not_of(ws);
    if (start == std::string::npos) {
        s.clear();
        return;
    }

    size_t end = s.find_last_not_of(ws);
    s = s.substr(start, end - start + 1);
}

void MtlParser::parseTextureFile(std::istringstream& iss)
{
	std::string file;
	std::getline(iss, file);
	trim(file);
	if (file.empty())
		throw ParseError("MtlParser: 'map_Kd' expect a file");
	_materials.at(_actualMaterial)._map_Kd = file;
	_materials.at(_actualMaterial)._hasTexture = true;
};

void MtlParser::parseIlluminationModel(std::istringstream& iss)
{
	int i;
	if (!(iss >> i))
		throw ParseError("MtlParser: 'illum' expect 1 floats values");
	std::string extra;
	if (iss >> extra)
		throw ParseError("MtlParser: Unexpected extra value: " + extra);
	_materials.at(_actualMaterial)._illum = i;
}