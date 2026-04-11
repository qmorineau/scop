#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "TextureLoader.hpp"
#include "Matrix4.hpp"

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    // Activate Shader
    void use();
    // Utility Uniform Functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
	void setMat4(const std::string &name, const mat4 &mat) const;
    void setVec3(const std::string &name, const vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
	void linkTexture(int i);

private:
	enum class Type
	{
		Vertex,
		Fragment
	};
    unsigned int _id;

	std::string		readShaderFile(const char* file);
	unsigned int	createShader(const char** shaderCode, Shader::Type type);
    void			checkCompileErrors(unsigned int shader, std::string type);
};

#endif