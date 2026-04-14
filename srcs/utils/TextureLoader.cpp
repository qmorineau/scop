#include "TextureLoader.hpp"

int TextureLoader::loadTexture(std::string file)
{
	std::cerr << "Loading Texture: " << file << std::endl;
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		GLenum format = (nrChannels == 4) ? GL_RGBA :
						(nrChannels == 3) ? GL_RGB  :
						(nrChannels == 1) ? GL_RED  : GL_RGB;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "File Loaded: " << file << " (" << width << "x" << height << ", channels=" << nrChannels << ", texID=" << texture << ")\n";
	}
	else
		throw LoadFileError(file);

	stbi_image_free(data);
	return texture;
}
