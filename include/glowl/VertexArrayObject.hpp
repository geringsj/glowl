/*
 * Mesh.hpp
 *
 * MIT License
 * Copyright (c) 2024 Michael Becher
 */

#ifndef GLOWL_VERTEX_ARRAY_HPP
#define GLOWL_VERTEX_ARRAY_HPP

#include <memory>
#include <string>
#include <vector>

#include "BufferObject.hpp"
#include "VertexLayout.hpp"
#include "glinclude.h"

namespace glowl
{

    /**
     * \class VertexArrayObject
     *
     * \brief Encapsulates a vertex array. Condensed from the Mesh class.
     *
     * \author Sergej Geringer
     */
    class VertexArrayObject
    {
    public:

        /**
         * \brief Constructor taking vertex attribute layout descriptors.
         *
         * Note: Active OpenGL context required for construction.
         * Use std::unqiue_ptr (or shared_ptr) for delayed construction of class member variables of this type.
         */
        VertexArrayObject(std::vector<VertexLayout> const& vertex_descriptor,
            GLsizei const   draw_items_count = 0,
            GLuint const    index_buffer_name = 0,
            GLenum const    index_data_type = GL_UNSIGNED_INT,
            GLenum const    primitive_type = GL_TRIANGLES);


        /**
         * \brief Vertex Array constructor initializing with zero .
         *
         * Note: Active OpenGL context required for construction.
         * Use std::unqiue_ptr (or shared_ptr) for delayed construction of class member variables of this type.
         */
        VertexArrayObject() : m_va_handle(0) {};

        ~VertexArrayObject()
        {
            glDeleteVertexArrays(1, &m_va_handle);
        }

        VertexArrayObject(const VertexArrayObject& cpy) = delete;
        VertexArrayObject& operator=(const VertexArrayObject& rhs) = delete;

        glowl_impl_move_swap_operators(VertexArrayObject)
        //VertexArrayObject(VertexArrayObject&& other);
        //VertexArrayObject& operator=(VertexArrayObject&& rhs);

        void bind() const
        {
            glBindVertexArray(m_va_handle);
        }

        /**
         * Draw function for your conveniences.
         * If you need/want to work with sth. different from glDrawElementsInstanced,
         * use bind() and do your own thing.
         */
        void draw(GLsizei instance_cnt = 1)
        {
            glBindVertexArray(m_va_handle);

            if(m_index_buffer_name && m_draw_items_count) {
                glDrawElementsInstanced(m_primitive_type, m_draw_items_count, m_index_type, nullptr, instance_cnt);
            } else {
                glDrawArraysInstanced(m_primitive_type, 0, m_draw_items_count, instance_cnt);
            }
            glBindVertexArray(0);
        }

        std::vector<VertexLayout> getVertexLayouts() const
        {
            return m_vertex_descriptor;
        }

        GLuint getDrawItemsCount() const
        {
            return m_draw_items_count;
        }

        GLenum getIndexType() const
        {
            return m_index_type;
        }

        GLenum getPrimitiveType() const
        {
            return m_primitive_type;
        }

        GLuint getHandle() const {
            return m_va_handle;
        }

    private:
        GLuint                    m_va_handle = 0;
        std::vector<VertexLayout> m_vertex_descriptor;
        GLenum m_primitive_type = 0;

        GLsizei m_draw_items_count = 0; // either indices (if index buffer given) or triangle count
        GLenum m_index_type = 0;
        GLuint m_index_buffer_name = 0;

        void createVertexArray();
        void checkError();

        void swap(VertexArrayObject& other) {
            glowl_swap_member(m_va_handle);
            glowl_swap_member(m_vertex_descriptor);
            glowl_swap_member(m_primitive_type);

            glowl_swap_member(m_draw_items_count);
            glowl_swap_member(m_index_type);
            glowl_swap_member(m_index_buffer_name);
        }
    };

    inline VertexArrayObject::VertexArrayObject(
            std::vector<VertexLayout> const& vertex_descriptor,
            GLsizei const   draw_items_count,
            GLuint const    index_buffer_name,
            GLenum const    index_data_type,
            GLenum const    primitive_type)
        : m_va_handle(0)
        , m_vertex_descriptor(vertex_descriptor)
        , m_primitive_type(primitive_type)
        , m_index_type(index_data_type)
        , m_index_buffer_name(index_buffer_name)
        , m_draw_items_count(draw_items_count)
    {
        createVertexArray();
        checkError();
    }

    inline void VertexArrayObject::createVertexArray()
    {
        glCreateVertexArrays(1, &m_va_handle);

        GLuint global_attrib_idx = 0;
        std::size_t vertex_layout_idx = 0;
        for (auto& descr: m_vertex_descriptor) {

            glVertexArrayVertexBuffer(
                m_va_handle,
                static_cast<GLuint>(vertex_layout_idx), // buffer binding index
                descr.buffer_name,
                descr.buffer_start_offset,
                descr.stride);

            for (auto& attribute: descr.attributes) {

                glEnableVertexArrayAttrib(m_va_handle, global_attrib_idx);
                switch (attribute.shader_input_type)
                {
                case GL_FLOAT:
                    glVertexArrayAttribFormat(m_va_handle,
                                              global_attrib_idx,
                                              attribute.size,
                                              attribute.type,
                                              attribute.normalized,
                                              attribute.offset);
                    break;
                case GL_INT:
                case GL_UNSIGNED_INT:
                    glVertexArrayAttribIFormat(m_va_handle,
                                               global_attrib_idx,
                                               attribute.size,
                                               attribute.type,
                                               attribute.offset);
                    break;
                case GL_DOUBLE:
                    glVertexArrayAttribLFormat(m_va_handle,
                                               global_attrib_idx,
                                               attribute.size,
                                               attribute.type,
                                               attribute.offset);
                    break;
                default:
                    throw MeshException(
                        "Mesh::createVertexArray - invalid vertex shader input type given (use float, double or int)");
                    break;
                }
                glVertexArrayAttribBinding(m_va_handle, global_attrib_idx, static_cast<GLuint>(vertex_layout_idx));

                global_attrib_idx++;
            }

            vertex_layout_idx++;
        }

        if (m_index_buffer_name) {
            glVertexArrayElementBuffer(m_va_handle, m_index_buffer_name);
        }
    }

    inline void VertexArrayObject::checkError()
    {
        auto err = glGetError();
        if (err != GL_NO_ERROR)
        {
            throw MeshException("Mesh::Mesh - OpenGL error " + std::to_string(err));
        }
    }

} // namespace glowl

#endif // GLOWL_VERTEX_ARRAY_HPP
