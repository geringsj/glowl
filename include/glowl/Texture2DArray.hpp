/*
 * Texture2DArray.hpp
 * 
 * MIT License
 * Copyright (c) 2019 Michael Becher
 */

#ifndef Texture2DArray_hpp
#define Texture2DArray_hpp

#include "Texture.hpp"

/**
* \class Texture2DArray
*
* \brief Encapsulates 2D texture array functionality.
*
* \author Michael Becher
*/
class Texture2DArray : public Texture
{
public:
	Texture2DArray(std::string id,
					TextureLayout const& layout,
					GLvoid* data,
					bool generateMipmap = false);
	Texture2DArray(const Texture2DArray&) = delete; // TODO: think of meaningful copy operation...maybe copy texture context to new texture object?
	Texture2DArray(Texture2DArray&& other) = delete;
	Texture2DArray& operator=(const Texture2DArray& rhs) = delete;
	Texture2DArray& operator=(Texture2DArray&& rhs) = delete;
	//~Texture2DArray();

	void bindTexture() const;

	void updateMipmaps();

	TextureLayout getTextureLayout() const;

	unsigned int getWidth() const;
	unsigned int getHeigth() const;
	unsigned int getLayers() const;

private:
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_layers;
};

#endif // !Texture2DArray_hpp
