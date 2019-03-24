#pragma once

#include <string>

class Texture2D
{
public:
	Texture2D(const std::string& imgPath, bool hasAlphaChannel);
	void Bind(unsigned int textureUnit = 0) const;

	inline int GetWidth() const { return m_imageWidth; }
	inline int GetHeight() const { return m_imageHeight; }
private:
	void CreateTexture(const char* imagePath);

private:
	unsigned int m_textureID;

	int m_imageWidth, m_imageHeight;
	bool m_hasAlphaChannel;

};
