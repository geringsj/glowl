/*
 * VertexLayout.hpp
 *
 * MIT License
 * Copyright (c) 2019 Michael Becher
 */

#ifndef GLOWL_VERTEXLAYOUT_HPP
#define GLOWL_VERTEXLAYOUT_HPP

#include "glinclude.h"

namespace glowl
{

    /**
     * \struct VertexLayout
     *
     * \brief Container for vertex layout descritions. Used by Mesh class.
     *
     * Usage hints (in combination with Mesh class):
     * Each VertexLayout instance is associated with exaclty one vertex buffer and describes the attributes
     * in that buffer. Thus, multiple vertex buffers require a matching number of layouts.
     * Give a correct stride value for the attributes (don't use 0 for tightly packed vertex data, DSA requires the
     * actual stride in bytes), e.g.:
     *
     * Non-interleaved, tightly packed vertex data,
     * e.g. two attribs in two buffers {vec3,vec3,...} and {vec2,vec2,...} have strides 12 and 8 in two diffrent vertex
     * layouts
     *
     * Partly interleaved (or non-tightly packed) vertex data,
     * e.g. three attribs in two buffers {vec3,vec3,...} and {{vec3,vec2},{vec3,vec2},...} have strides 12 and 20 in two
     * different vertex layouts
     *
     * Fully interleaved vertex data,
     * e.g. three attribs in one buffer {{vec3,vec3,vec2},{vec3,vec3,vec2},...} have stride 32 in a single vertex layout
     *
     * \author Michael Becher
     */
    struct VertexLayout
    {
        struct Attribute
        {
            Attribute(GLint     size,
                      GLenum    type,
                      GLboolean normalized,
                      GLsizei   offset,
                      GLenum    shader_input_type = GL_FLOAT)
                : size(size),
                  type(type),
                  normalized(normalized),
                  offset(offset),
                  shader_input_type(shader_input_type)
            {
            }

            // according to https://registry.khronos.org/OpenGL-Refpages/gl4/html/glVertexAttribFormat.xhtml
            GLint     size; ///< number of components per vertex, must be 1, 2, 3, 4, or GL_BGRA
            GLenum    type; ///< type of the data stored in the array. type is one of GL_BYTE, GL_SHORT, GL_INT, GL_FIXED, GL_FLOAT, GL_HALF_FLOAT, and GL_DOUBLE indicate types GLbyte, GLshort, GLint, GLfixed, GLfloat, GLhalf, and GLdouble, respectively [...]
            GLboolean normalized; ///< if normalized is GL_TRUE, then integer data is normalized to the range [-1, 1] or [0, 1] if it is signed or unsigned, respectively. If normalized is GL_FALSE then integer data is directly converted to floating point.
            GLsizei   offset; ///< relativeoffset is the offset, measured in basic machine units of the first element relative to the start of the vertex buffer binding this attribute fetches from
            GLenum    shader_input_type; ///< type used by vertex shader input: float, double or integer
                        //  
        };

        VertexLayout() : attributes() {}
        /**
         * Construct VertexLayout from set of strides and attributes
         *
         * \param strides Stride values in byte per vertex attribute. It is possible to use only a single stride value
         * for all attributes (see VertexLayout member documentation).
         *
         */
        VertexLayout(GLsizei stride, std::vector<Attribute> const& attributes) : stride(stride), attributes(attributes)
        {
        }
        /**
         * Construct VertexLayout from set of strides and attributes
         *
         * \param strides Stride values in byte per vertex attribute. It is possible to use only a single stride value
         * for all attributes (see VertexLayout member documentation).
         *
         */
        VertexLayout(GLsizei stride, std::vector<Attribute>&& attributes) : stride(stride), attributes(attributes) {}

        /**
         * Construct VertexLayout from buffer name, buffer offset, buffer stride and set of attributes contained in the buffer
         * No new buffers are created, vertex attribute data is sourced from the provided buffer
         *
         * \param strides Stride values in byte per vertex attribute. It is possible to use only a single stride value
         * for all attributes (see VertexLayout member documentation).
         *
         */
        VertexLayout(
            GLuint  buffer_name,
            GLsizei buffer_stride,
            GLsizei buffer_start_offset,
            std::vector<Attribute> const& attributes) 
            : stride(buffer_stride)
            , buffer_start_offset(buffer_start_offset)
            , buffer_name(buffer_name)
            , attributes(attributes)
        {}

        GLsizei                stride = 0; // buffer_element_stride
        GLintptr               buffer_start_offset = 0; // buffer_element_start_offset
        GLuint                 buffer_name = 0; // buffer GL handle
        std::vector<Attribute> attributes;
    };

    inline bool operator==(VertexLayout::Attribute const& lhs, VertexLayout::Attribute const& rhs)
    {
        return lhs.normalized == rhs.normalized && lhs.offset == rhs.offset && lhs.size == rhs.size &&
               lhs.type == rhs.type && lhs.shader_input_type == rhs.shader_input_type;
    }

    inline bool operator==(VertexLayout const& lhs, VertexLayout const& rhs)
    {
        bool rtn = true;

        rtn &= lhs.stride == rhs.stride;

        if (lhs.attributes.size() == rhs.attributes.size())
        {
            for (std::size_t i = 0; i < lhs.attributes.size(); ++i)
            {
                rtn &= (lhs.attributes[i] == rhs.attributes[i]);
            }
        }
        else
        {
            rtn = false;
        }

        return rtn;
    }

    inline constexpr std::size_t computeByteSize(GLenum value_type)
    {
        std::size_t retval = 0;

        switch (value_type)
        {
        case GL_BYTE:
            retval = 1;
            break;
        case GL_SHORT:
            retval = 2;
            break;
        case GL_INT:
            retval = 4;
            break;
        case GL_FIXED:
            retval = 4;
            break;
        case GL_FLOAT:
            retval = 4;
            break;
        case GL_HALF_FLOAT:
            retval = 2;
            break;
        case GL_DOUBLE:
            retval = 8;
            break;
        case GL_UNSIGNED_BYTE:
            retval = 1;
            break;
        case GL_UNSIGNED_SHORT:
            retval = 2;
            break;
        case GL_UNSIGNED_INT:
            retval = 4;
            break;
        case GL_INT_2_10_10_10_REV:
            retval = 4;
            break;
        case GL_UNSIGNED_INT_2_10_10_10_REV:
            retval = 4;
            break;
        case GL_UNSIGNED_INT_10F_11F_11F_REV:
            retval = 4;
            break;
        default:
            break;
        }

        return retval;
    }

    inline std::size_t computeAttributeByteSize(VertexLayout::Attribute attrib_desc)
    {
        return computeByteSize(attrib_desc.type) * attrib_desc.size;
    }

} // namespace glowl

#endif // GLOWL_VERTEXLAYOUT_HPP
