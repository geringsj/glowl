#include "Texture2DArray.hpp"

#include <algorithm>
#include <iostream>

Texture2DArray::Texture2DArray(std::string id,
	TextureLayout const& layout,
	GLvoid* data,
	bool generateMipmap)
	: Texture(id, layout.internal_format, layout.format, layout.type, layout.levels), m_width(layout.width), m_height(layout.height), m_layers(layout.depth)
{
	glGenTextures(1, &m_name);

	glBindTexture(GL_TEXTURE_2D_ARRAY, m_name);

	for (auto& pname_pvalue : layout.int_parameters)
		glTexParameteri(GL_TEXTURE_2D_ARRAY, pname_pvalue.first, pname_pvalue.second);
	
	for (auto& pname_pvalue : layout.float_parameters)
		glTexParameterf(GL_TEXTURE_2D_ARRAY, pname_pvalue.first, pname_pvalue.second);

	GLsizei levels = 1;

	levels = std::min(layout.levels, 1 + static_cast<GLsizei>(floor(log2(std::max(m_width, m_height)))));

	glTexStorage3D(GL_TEXTURE_2D_ARRAY, levels, m_internal_format, m_width, m_height, m_layers);

	if(data != nullptr)
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, m_width, m_height, m_layers, m_format, m_type, data);

	if (generateMipmap)
		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	m_texture_handle = glGetTextureHandleARB(m_name);

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		// "Do something cop!"
		std::cerr << "GL error during array texture (id: " << id << ") creation: " << err << std::endl;
	}
}

void Texture2DArray::bindTexture() const
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_name);
}

void Texture2DArray::updateMipmaps()
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_name);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

TextureLayout Texture2DArray::getTextureLayout() const
{
	return TextureLayout(m_internal_format, m_width, m_height, m_layers, m_format, m_type, m_levels);
}

unsigned int Texture2DArray::getWidth() const
{
	return m_width;
}

unsigned int Texture2DArray::getHeigth() const
{
	return m_height;
}

unsigned int Texture2DArray::getLayers() const
{
	return m_layers;
}