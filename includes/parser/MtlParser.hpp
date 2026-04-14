#ifndef MTLPARSER_HPP
#define MTLPARSER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>

#include "Material.hpp"

class MtlParser
{
	class ParseError : public std::runtime_error
	{
		public:
			explicit ParseError(const std::string& msg) : std::runtime_error(msg) {}
	};
	public:
		MtlParser(std::string path, std::string file) : _path(path), _fileName(file), _file(path + file)
		{
			if (!_file.is_open())
				throw ParseError("MtlParser: Can't open \"" + file + "\"");
		};
		~MtlParser() {};

		std::unordered_map<std::string, Material> parse();

		void createNewMaterial(std::istringstream& iss);

		void parseSpecularExponent(std::istringstream& iss);
		void parseAmbiantColor(std::istringstream& iss);
		void parseDiffuseColor(std::istringstream& iss);
		void parseSpecularColor(std::istringstream& iss);
		void parseEmissiveColor(std::istringstream& iss);
		void parseOpticalDensity(std::istringstream& iss);
		void parseTransparency(std::istringstream& iss);
		void parseTextureFile(std::istringstream& iss);
		void parseIlluminationModel(std::istringstream& iss);
	private:
		std::string									_path;
		std::string									_fileName;
		std::ifstream								_file;
		std::string									_actualMaterial;
		std::unordered_map<std::string, Material>	_materials;
};

#endif