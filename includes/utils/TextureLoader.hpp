#ifndef TEXTURELOADER_HPP
#define TEXTURELOADER_HPP

#include <string>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_images.h"
#include "Shader.hpp"

class TextureLoader
{
	public:
		class LoadFileError : public std::exception
		{
			public:
				explicit LoadFileError(const std::string& msg) : _msg("TextureLoader: failed to load file: " + msg) {}

				const char* what() const noexcept override {
					return _msg.c_str();
				}

			private:
				std::string _msg;
		};
		TextureLoader() {};

		int loadTexture(std::string file);
};

#endif