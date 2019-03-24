#include "Texture2D.h"

#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>

Texture2D::Texture2D(const std::string& imagePath, bool hasAlphaChannel)
	: m_hasAlphaChannel(hasAlphaChannel)
{
	CreateTexture(imagePath.c_str());
}

void Texture2D::Bind(unsigned int textureUnit) const
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture2D::CreateTexture(const char* imgPath)
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load images with bottom left as origin instead of top left
	stbi_set_flip_vertically_on_load(true);
	int nrChannels;
	unsigned char* imgData = stbi_load(imgPath, &m_imageWidth, &m_imageHeight, &nrChannels,0);

	if (unsigned int format = (m_hasAlphaChannel == true ? GL_RGBA : GL_RGB); imgData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_imageWidth, m_imageHeight, 0, format, GL_UNSIGNED_BYTE, imgData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Error: unable to load texture: " << imgPath << std::endl;

	stbi_image_free(imgData);
}
