#ifndef texture2D_h
#define texture2D_h

#include "texture.h"


/**
* \class Texture2D
*
* \brief Encapsulates basic 2D texure functionality.
*
* This class encapsulates basic 2D functionality including creation of a 2D texture,
* as well as setting parameters and binding it.
*
* \author Michael Becher
*
* \date 6th November 2015
*/
class Texture2D : public Texture
{
public:
	/**
	* \brief Constructor that creates and loads a 2D texture.
	* \param internal_format Specifies the internal format of the texture (e.g. GL_RGBA32F)
	* \param dim_x Specifies the width of the texture in pixels.
	* \param dim_y Specifies the height of the texture in pixels.
	* \param format Specifies the format of the texture (e.g. GL_RGBA)
	* \param type Specifies the type of the texture (e.g. GL_FLOAT)
	* \param data Pointer to the actual texture data.
	*/
	Texture2D(std::string name, GLint internal_format, unsigned int width, unsigned int height, GLenum format, GLenum type, GLvoid * data);
	Texture2D(const Texture2D &) = delete;

	/**
	* \brief Bind the texture.
	*/
	void bindTexture() const;

	/**
	* \brief Set a texture parameter.
	* \param pname Specifies the parameter to be set.
	* \param param Specifies the value that the parameter is set to.
	*/
	void texParameteri(GLenum pname, GLenum param);

	/**
	 * \brief Set several texture parameters at once
	 * \param pname_param_pairs List of parameter name and parameter value pairs (e.g. {{GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE},{...,...},...}
	 */
	template<typename Container>
	void texParameteri(const Container& pname_param_pairs)
	{
		glBindTexture(GL_TEXTURE_2D, m_handle);

		for(auto& pname_param : pname_param_pairs)
			glTexParameteri(GL_TEXTURE_2D, pname_param.first, pname_param.second);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	/**
	* \brief Reload the texture with a new size but unchanged format and type.
	* \note Will not work if load() hasn't been called on this object yet.
	* \param width Specifies the new width of the texture in pixels.
	* \param height Specifies the new height of the texture in pixels.
	* \param data Pointer to the new texture data.
	* \return Returns true if the texture was succesfully created, false otherwise
	*/
	bool reload(unsigned int width, unsigned int height, GLvoid * data);

	/**
	* \brief Reload the texture with any new format, type and size.
	* \note Will not work if load() hasn't been called on this object yet.
	* \param internal_format Specifies the new internal format of the texture (e.g. GL_RGBA)
	* \param width Specifies the new width of the texture in pixels.
	* \param height Specifies the new height of the texture in pixels.
	* \param format Specifies the new format of the texture (e.g. GL_RGBA32F)
	* \param type Specifies the new type of the texture (e.g. GL_FLOAT)
	* \param data Pointer to the new texture data.
	* \return Returns true if the texture was succesfully created, false otherwise
	*/
	bool reload(GLenum internal_format, unsigned int width, unsigned int height, GLenum format, GLenum type, GLvoid * data);

	unsigned int getWidth();

	unsigned int getHeight();

private:
	unsigned int m_width;
	unsigned int m_height;
};

#endif
