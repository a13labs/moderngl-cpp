
/*
   Copyright 2022 Alexandre Pires (c.alexandre.pires@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include "mgl_opengl/vertex_array.hpp"
#include "mgl_opengl/buffer.hpp"
#include "mgl_opengl/buffer_layout.hpp"
#include "mgl_opengl/context.hpp"
#include "mgl_opengl/program.hpp"

#include "mgl_core/debug.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  vertex_array::vertex_array(const context_ref& ctx,
                             program_ref program,
                             mgl::opengl::vertex_buffer_list vertex_buffers,
                             buffer_ref index_buffer,
                             int32_t index_element_size,
                             bool skip_errors,
                             mgl::opengl::render_mode mode)
      : gl_object(ctx)
  {
    MGL_CORE_ASSERT(program, "program is null");
    MGL_CORE_ASSERT(!program->released(), "program already released");
    MGL_CORE_ASSERT(index_element_size == 1 || index_element_size == 2 || index_element_size == 4,
                    "index_element_size must be 1, 2, or 4");
    MGL_CORE_ASSERT(program->ctx() == ctx, "program context mismatch");

#ifdef MGL_CORE_ENABLE_ASSERTS
    int32_t i = 0;
    for(auto&& v_data : vertex_buffers)
    {
      if(v_data.buffer == nullptr)
      {
        MGL_CORE_ASSERT(false, "vertex_buffers[{0}]: empty vertex buffer", i);
        return;
      }

      buffer_layout layout = v_data.buffer_layout;

      if(layout.is_invalid())
      {
        MGL_CORE_ASSERT(false, "vertex_buffers[{0}]: invalid buffer layout", i);
        return;
      }

      if(!v_data.attributes.size())
      {
        MGL_CORE_ASSERT(false, "vertex_buffers[{0}]: attributes must not be empty", i);
        return;
      }

      if((int32_t)v_data.attributes.size() != layout.size())
      {
        MGL_CORE_ASSERT(false,
                        "vertex_buffers[{0}]: format and attributes size mismatch {1} != {2}",
                        i,
                        layout.size(),
                        v_data.attributes.size());
        return;
      }

      i++;
    }
#endif

    m_num_vertices = 0;
    m_num_instances = 1;
    m_program = program;
    m_index_buffer = index_buffer;
    m_index_element_size = index_element_size;
    const int32_t element_types[5] = { 0, GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, 0, GL_UNSIGNED_INT };
    m_index_element_type = element_types[index_element_size];
    m_num_vertices = -1;

    GLuint glo = 0;
    glGenVertexArrays(1, &glo);

    if(!glo)
    {
      MGL_CORE_ASSERT(glo, "cannot create vertex array");
      return;
    }

    gl_object::set_glo(glo);

    glBindVertexArray(gl_object::glo());

    if(m_index_buffer != nullptr)
    {
      m_num_vertices = (int32_t)(m_index_buffer->size() / index_element_size);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer->glo());
    }

    i = 0;
    for(auto&& v_data : vertex_buffers)
    {
      auto buffer = v_data.buffer;
      const char* format = v_data.buffer_layout.c_str();

      buffer_layout layout = v_data.buffer_layout;

      int32_t buf_vertices = (int32_t)(buffer->size() / layout.size());

      if(!layout.divisor() && m_index_buffer == nullptr && (!i || m_num_vertices > buf_vertices))
      {
        m_num_vertices = buf_vertices;
      }

      glBindBuffer(GL_ARRAY_BUFFER, buffer->glo());

      for(size_t j = 0; j < v_data.attributes.size(); ++j)
      {
        if(!m_program->has_attribute(v_data.attributes[j]))
        {
          continue;
        }
        auto attr = m_program->get_attribute(v_data.attributes[j]);

        buffer_layout::element element = layout[j];
        int32_t attribute_location = attr.location;
        int32_t attribute_rows_length = attr.data_type->rows_length;
        int32_t attribute_scalar_type = attr.data_type->scalar_type;

        char* ptr = (char*)(intptr_t)element.offset;
        for(int32_t r = 0; r < attribute_rows_length; ++r)
        {
          int32_t location = attribute_location + r;
          int32_t count = element.count / attribute_rows_length;

          switch(attribute_scalar_type)
          {
            case GL_FLOAT:
              glVertexAttribPointer(
                  location, count, element.type, element.normalize, layout.stride(), ptr);
              break;
            case GL_DOUBLE:
              glVertexAttribLPointer(location, count, element.type, layout.stride(), ptr);
              break;
            case GL_INT:
              glVertexAttribIPointer(location, count, element.type, layout.stride(), ptr);
              break;
            case GL_UNSIGNED_INT:
              glVertexAttribIPointer(location, count, element.type, layout.stride(), ptr);
              break;
          }

          glVertexAttribDivisor(location, layout.divisor());
          glEnableVertexAttribArray(location);
          ptr += element.size / attribute_rows_length;
        }
      }
      i++;
    }
  }

  void vertex_array::release()
  {
    MGL_CORE_ASSERT(!gl_object::released(), "Vertex Array already released");
    GLuint glo = gl_object::glo();
    glDeleteVertexArrays(1, &glo);
    gl_object::set_glo(GL_ZERO);
  }

  void vertex_array::render(mgl::opengl::render_mode mode, int vertices, int first, int instances)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "Vertex Array already released");

    if(vertices == 0)
    {
      MGL_CORE_ASSERT(m_num_vertices > 0, "cannot detect the number of vertices");
      vertices = m_num_vertices;
    }

    if(instances == 0)
    {
      instances = m_num_instances;
    }

    glUseProgram(m_program->glo());
    glBindVertexArray(gl_object::glo());

    if(m_index_buffer != nullptr)
    {
      const void* ptr = (const void*)((GLintptr)first * m_index_element_size);
      glDrawElementsInstanced(mode, vertices, m_index_element_type, ptr, instances);
    }
    else
    {
      glDrawArraysInstanced(mode, first, vertices, instances);
    }
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "OpenGL error");
  }

  void vertex_array::render_indirect(const buffer_ref& buffer,
                                     mgl::opengl::render_mode mode,
                                     int count,
                                     int first)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "Vertex Array already released");

    glUseProgram(m_program->glo());
    glBindVertexArray(gl_object::glo());
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buffer->glo());

    const void* ptr = (const void*)((GLintptr)first * 20);

    if(m_index_buffer != nullptr)
    {
      glMultiDrawElementsIndirect(mode, m_index_element_type, ptr, count, 20);
    }
    else
    {
      glMultiDrawArraysIndirect(mode, ptr, count, 20);
    }
  }

  void vertex_array::transform(const mgl::ref_list<buffer>& buffers,
                               mgl::opengl::render_mode mode,
                               int vertices,
                               int first,
                               int instances,
                               int buffer_offset)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "Vertex Array already released");

    MGL_CORE_ASSERT(m_program->num_varyings(), "the program has no varyings")

    if(vertices < 0)
    {
      MGL_CORE_ASSERT(m_num_vertices >= 0, "cannot detect the number of vertices");
      vertices = m_num_vertices;
    }

    if(instances < 0)
    {
      instances = m_num_instances;
    }

    int output_mode = -1;

    // If a geo shader is present we need to sanity check the the rendering mode
    if(m_program->geometry_output() > -1)
    {
      output_mode = m_program->geometry_output();

      // The rendering mode must match the input type in the geometry shader
      // points, lines, lines_adjacency, triangles, triangles_adjacency
#ifdef MGL_CORE_ENABLE_ASSERTS
      switch(m_program->geometry_input())
      {
        case GL_POINTS:
          MGL_CORE_ASSERT(mode == GL_POINTS,
                          "Geometry shader expects POINTS as input. Change the "
                          "transform mode.");
          break;
        case GL_LINES:
          MGL_CORE_ASSERT(mode == GL_LINES || mode == GL_LINE_STRIP || mode == GL_LINE_LOOP ||
                              mode == GL_LINES_ADJACENCY,
                          "Geometry shader expects LINES, LINE_STRIP, GL_LINE_LOOP or "
                          "GL_LINES_ADJACENCY as input. Change the rendering mode.");
          break;
        case GL_LINES_ADJACENCY:
          MGL_CORE_ASSERT(mode == GL_LINES_ADJACENCY || mode == GL_LINE_STRIP_ADJACENCY,
                          "Geometry shader expects LINES_ADJACENCY or LINE_STRIP_ADJACENCY "
                          "as input. Change the rendering mode.");
          break;
        case GL_TRIANGLES:
          MGL_CORE_ASSERT(mode == GL_TRIANGLES || mode == GL_TRIANGLE_STRIP ||
                              mode == GL_TRIANGLE_FAN,
                          "Geometry shader expects GL_TRIANGLES, GL_TRIANGLE_STRIP "
                          "or GL_TRIANGLE_FAN as input. Change the rendering mode.");
          break;
        case GL_TRIANGLES_ADJACENCY:
          MGL_CORE_ASSERT(mode == GL_TRIANGLES_ADJACENCY || mode == GL_TRIANGLE_STRIP_ADJACENCY,
                          "Geometry shader expects GL_TRIANGLES_ADJACENCY or "
                          "GL_TRIANGLE_STRIP_ADJACENCY as input. Change the rendering mode.");
          break;
        default:
          MGL_CORE_ERROR("Unexpected geometry shader input mode: %d", m_program->geometry_input());
          return;
          break;
      }
#endif
    }
    else
    {
      // If no geometry shader is present we need to determine the output mode by
      // looking at the input
      switch(mode)
      {
        case GL_POINTS: output_mode = GL_POINTS; break;
        case GL_LINES:
        case GL_LINE_LOOP:
        case GL_LINE_STRIP:
        case GL_LINES_ADJACENCY:
        case GL_LINE_STRIP_ADJACENCY: output_mode = GL_LINES; break;
        case GL_TRIANGLES:
        case GL_TRIANGLE_STRIP:
        case GL_TRIANGLE_FAN:
        case GL_TRIANGLES_ADJACENCY:
        case GL_TRIANGLE_STRIP_ADJACENCY: output_mode = GL_TRIANGLES; break;
        default: MGL_CORE_ASSERT(false, "Primitive mode not supported"); return;
      }
    }

    glUseProgram(m_program->glo());
    glBindVertexArray(gl_object::glo());

    int i = 0;
    for(auto&& buffer : buffers)
    {
      glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER,
                        i,
                        buffer->glo(),
                        buffer_offset,
                        buffer->size() - buffer_offset);
      i++;
    }

    glEnable(GL_RASTERIZER_DISCARD);
    glBeginTransformFeedback(output_mode);

    if(m_index_buffer != nullptr)
    {
      const void* ptr = (const void*)((GLintptr)first * m_index_element_size);
      glDrawElementsInstanced(mode, vertices, m_index_element_type, ptr, instances);
    }
    else
    {
      glDrawArraysInstanced(mode, first, vertices, instances);
    }

    glEndTransformFeedback();
    if(gl_object::ctx()->enable_flags() & mgl::opengl::enable_flag::RASTERIZER_DISCARD)
    {
      glDisable(GL_RASTERIZER_DISCARD);
    }
    glFlush();
  }

  void vertex_array::bind(int location,
                          const char* type,
                          const buffer_ref& buffer,
                          const char* format,
                          size_t offset,
                          int stride,
                          int divisor,
                          bool normalize)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "Vertex Array already released");

    MGL_CORE_ASSERT(!(type[0] == 'f' && normalize), "invalid normalize");

    buffer_layout layout = buffer_layout(format);

#ifdef MGL_CORE_ENABLE_ASSERTS
    MGL_CORE_ASSERT(!(layout.is_invalid() || layout.divisor() || layout.size() != 1),
                    "invalid format");
#endif

    buffer_layout::element element = layout[0];
    MGL_CORE_ASSERT(element.type, "invalid format");

    char* ptr = (char*)offset;

    glBindVertexArray(gl_object::glo());
    glBindBuffer(GL_ARRAY_BUFFER, buffer->glo());

    switch(type[0])
    {
      case 'f':
        glVertexAttribPointer(location, element.offset, element.type, normalize, stride, ptr);
        break;
      case 'i': glVertexAttribIPointer(location, element.offset, element.type, stride, ptr); break;
      case 'd': glVertexAttribLPointer(location, element.offset, element.type, stride, ptr); break;
      default: MGL_CORE_ASSERT(false, "invalid type"); return;
    }

    glVertexAttribDivisor(location, divisor);
    glEnableVertexAttribArray(location);
  }

  void vertex_array::set_index_buffer(const buffer_ref& value)
  {
    m_index_buffer = value;
    m_num_vertices = (int)(m_index_buffer->size() / 4);
  }

} // namespace  mgl::opengl
