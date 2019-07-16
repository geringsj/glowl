/*
 * ShaderStorageBufferObject.cpp
 * 
 * MIT License
 * Copyright (c) 2019 Michael Becher
 */

#include "ShaderStorageBufferObject.hpp"

ShaderStorageBufferObject::ShaderStorageBufferObject(unsigned int size, const GLvoid * data)
	: m_handle(0), m_size(size), m_written_size(0)
{
	/* make clang++ compiler 'unused variable' warning go away */
	if(0 && m_written_size)
	{
	}

	glGenBuffers(1, &m_handle);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_handle);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

ShaderStorageBufferObject::~ShaderStorageBufferObject()
{
	glDeleteBuffers(1,&m_handle);
}

void ShaderStorageBufferObject::reload(unsigned int size, GLuint index, const GLvoid * data)
{
	m_size = size;

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_handle);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	auto err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cerr << "Error - SSBO - reload: " << err << std::endl;
	}
}

void ShaderStorageBufferObject::bind()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_handle);
}

void ShaderStorageBufferObject::bind(GLuint index)
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_handle);
}

GLuint ShaderStorageBufferObject::getSize()
{
	return m_size;
}
