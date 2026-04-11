#include "Shader.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode = readShaderFile(vertexPath);
	std::string fragmentCode = readShaderFile(fragmentPath);

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	// Compile shader
	unsigned int vertex = createShader(&vShaderCode, Type::Vertex);
	checkCompileErrors(vertex, "VERTEX");
	unsigned int fragment = createShader(&fShaderCode, Type::Fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// Shader Program
	_id = glCreateProgram();
	glAttachShader(_id, vertex);
	glAttachShader(_id, fragment);
	glLinkProgram(_id);
	checkCompileErrors(_id, "PROGRAM");
	// Delete Shader, not need anymore
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

std::string Shader::readShaderFile(const char* file)
{
	std::string			shaderCode;
	std::ifstream		shaderFile;
	std::stringstream	shaderStream;

	// ensure ifstream objects can throw exceptions:
	shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	try 
	{
		shaderFile.open(file);
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
	}
	return shaderCode;
}

unsigned int Shader::createShader(const char** shaderCode, Shader::Type type)
{
	unsigned int shaderId;

	switch (type)
	{
		case Type::Vertex:
			shaderId = glCreateShader(GL_VERTEX_SHADER);
			break;
		case Type::Fragment:
			shaderId = glCreateShader(GL_FRAGMENT_SHADER);
			break;
	}
	glShaderSource(shaderId, 1, shaderCode, NULL);
	glCompileShader(shaderId);
	return shaderId;
}

// Activate Shader

void Shader::use() 
{ 
	glUseProgram(_id); 
}

// Uniform Utility

void Shader::setBool(const std::string &name, bool value) const
{         
	glUniform1i(glGetUniformLocation(_id, name.c_str()), (int)value); 
}

void Shader::setInt(const std::string &name, int value) const
{ 
	glUniform1i(glGetUniformLocation(_id, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const
{ 
	glUniform1f(glGetUniformLocation(_id, name.c_str()), value); 
}

void Shader::setMat4(const std::string &name, const mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, mat.dataPtr());
}
	
void Shader::setVec3(const std::string &name, const vec3 &value) const
{ 
	glUniform3fv(glGetUniformLocation(_id, name.c_str()), 1, &value.x); 
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{ 
	glUniform3f(glGetUniformLocation(_id, name.c_str()), x, y, z); 
}

void Shader::linkTexture(int i)
{
	setInt("material_mapKd", i);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}