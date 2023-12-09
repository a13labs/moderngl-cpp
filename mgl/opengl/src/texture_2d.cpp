
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
#include "mgl_opengl/texture_2d.hpp"
#include "mgl_opengl/buffer.hpp"
#include "mgl_opengl/context.hpp"
#include "mgl_opengl/data_type.hpp"
#include "mgl_opengl/framebuffer.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/math.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  void texture_2d::release()
  {
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    if(m_released)
    {
      return;
    }

    m_released = true;
    glDeleteTextures(1, (GLuint*)&m_texture_obj);
  }

  attachment::type texture_2d::attachment_type()
  {
    return attachment::TEXTURE;
  }

  texture::type texture_2d::texture_type()
  {
    return texture::TEXTURE_2D;
  }

  bool texture_2d::read_into(mgl::mem_buffer<uint8_t>& dst,
                             int level,
                             int alignment,
                             size_t write_offset)
  {
    MGL_CORE_ASSERT(!m_released, "Texture2D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8,
                    "alignment must be 1, 2, 4 or 8");
    MGL_CORE_ASSERT(level < m_max_level, "invalid level");
    MGL_CORE_ASSERT(!m_samples, "multisample textures cannot be read directly");

    int width = m_width / (1 << level);
    int height = m_height / (1 << level);

    width = width > 1 ? width : 1;
    height = height > 1 ? height : 1;

    size_t expected_size = width * m_components * m_data_type->size;
    expected_size = (expected_size + alignment - 1) / alignment * alignment;
    expected_size = expected_size * height;

    MGL_CORE_ASSERT(dst.size() >= write_offset + expected_size, "out of bounds");

    int pixel_type = m_data_type->gl_type;
    int base_format = m_depth ? GL_DEPTH_COMPONENT : m_data_type->base_format[m_components];

    char* ptr = (char*)dst.data() + write_offset;

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D, m_texture_obj);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glGetTexImage(GL_TEXTURE_2D, level, base_format, pixel_type, ptr);

    return glGetError() == GL_NO_ERROR;
  }

  bool texture_2d::read_into(buffer_ref& dst, int level, int alignment, size_t write_offset)
  {
    MGL_CORE_ASSERT(!m_released, "Texture2D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8,
                    "alignment must be 1, 2, 4 or 8");
    MGL_CORE_ASSERT(level < m_max_level, "invalid level");
    MGL_CORE_ASSERT(!m_samples, "multisample textures cannot be read directly");

    int width = m_width / (1 << level);
    int height = m_height / (1 << level);

    width = width > 1 ? width : 1;
    height = height > 1 ? height : 1;

    int pixel_type = m_data_type->gl_type;
    int base_format = m_depth ? GL_DEPTH_COMPONENT : m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_PACK_BUFFER, dst->m_buffer_obj);
    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D, m_texture_obj);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glGetTexImage(GL_TEXTURE_2D, level, base_format, pixel_type, (void*)write_offset);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    return glGetError() == GL_NO_ERROR;
  }

  bool texture_2d::write(const mgl::mem_buffer<uint8_t>& src,
                         const mgl::rect& viewport,
                         int level,
                         int alignment)
  {
    MGL_CORE_ASSERT(!m_released, "Texture2D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8,
                    "alignment must be 1, 2, 4 or 8");
    MGL_CORE_ASSERT(level < m_max_level, "invalid level");
    MGL_CORE_ASSERT(!m_samples, "multisample textures cannot be read directly");

    int x = viewport.x;
    int y = viewport.y;
    int width = viewport.width;
    int height = viewport.height;

    size_t expected_size = width * m_components * m_data_type->size;
    expected_size = (expected_size + alignment - 1) / alignment * alignment;
    expected_size = expected_size * height;

    MGL_CORE_ASSERT(src.size() >= expected_size, "out of bounds");

    int pixel_type = m_data_type->gl_type;
    int format = m_depth ? GL_DEPTH_COMPONENT : m_data_type->base_format[m_components];

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D, m_texture_obj);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glTexSubImage2D(GL_TEXTURE_2D, level, x, y, width, height, format, pixel_type, src.data());

    return glGetError() == GL_NO_ERROR;
  }

  bool texture_2d::write(const mgl::mem_buffer<uint8_t>& src, int level, int alignment)
  {
    MGL_CORE_ASSERT(!m_released, "Texture2D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8,
                    "alignment must be 1, 2, 4 or 8");
    MGL_CORE_ASSERT(level < m_max_level, "invalid level");
    MGL_CORE_ASSERT(!m_samples, "multisample textures cannot be read directly");

    int x = 0;
    int y = 0;
    int width = m_width / (1 << level);
    int height = m_height / (1 << level);

    width = width > 1 ? width : 1;
    height = height > 1 ? height : 1;

    size_t expected_size = width * m_components * m_data_type->size;
    expected_size = (expected_size + alignment - 1) / alignment * alignment;
    expected_size = expected_size * height;

    MGL_CORE_ASSERT(src.size() >= expected_size, "out of bounds");

    int pixel_type = m_data_type->gl_type;
    int format = m_depth ? GL_DEPTH_COMPONENT : m_data_type->base_format[m_components];

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D, m_texture_obj);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glTexSubImage2D(GL_TEXTURE_2D, level, x, y, width, height, format, pixel_type, src.data());

    return glGetError() == GL_NO_ERROR;
  }

  bool texture_2d::write(const buffer_ref& src,
                         const mgl::rect& viewport,
                         int level,
                         int alignment)
  {
    MGL_CORE_ASSERT(!m_released, "Texture2D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8,
                    "alignment must be 1, 2, 4 or 8");
    MGL_CORE_ASSERT(level < m_max_level, "invalid level");
    MGL_CORE_ASSERT(!m_samples, "multisample textures cannot be read directly");

    int x = viewport.x;
    int y = viewport.y;
    int width = viewport.width;
    int height = viewport.height;

    int pixel_type = m_data_type->gl_type;
    int format = m_depth ? GL_DEPTH_COMPONENT : m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, src->m_buffer_obj);
    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D, m_texture_obj);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glTexSubImage2D(GL_TEXTURE_2D, level, x, y, width, height, format, pixel_type, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    return glGetError() == GL_NO_ERROR;
  }

  bool texture_2d::write(const buffer_ref& src, int level, int alignment)
  {
    MGL_CORE_ASSERT(!m_released, "Texture2D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8,
                    "alignment must be 1, 2, 4 or 8");
    MGL_CORE_ASSERT(level < m_max_level, "invalid level");
    MGL_CORE_ASSERT(!m_samples, "multisample textures cannot be read directly");

    int x = 0;
    int y = 0;
    int width = m_width / (1 << level);
    int height = m_height / (1 << level);

    width = width > 1 ? width : 1;
    height = height > 1 ? height : 1;

    int pixel_type = m_data_type->gl_type;
    int format = m_depth ? GL_DEPTH_COMPONENT : m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, src->m_buffer_obj);
    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D, m_texture_obj);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glTexSubImage2D(GL_TEXTURE_2D, level, x, y, width, height, format, pixel_type, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    return glGetError() == GL_NO_ERROR;
  }

  void texture_2d::bind_to_image(int unit, bool read, bool write, int level, int format)
  {
    MGL_CORE_ASSERT(!m_released, "Texture2D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(read || write, "Illegal access mode. Read or write needs to be enabled.");

    int access = GL_READ_WRITE;
    if(read && !write)
      access = GL_READ_ONLY;
    else if(!read && write)
      access = GL_WRITE_ONLY;

    int frmt = format ? format : m_data_type->internal_format[m_components];

    glBindImageTexture(unit, m_texture_obj, level, 0, 0, access, frmt);
  }

  void texture_2d::use(int index)
  {
    MGL_CORE_ASSERT(!m_released, "Texture2D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(texture_target, m_texture_obj);
  }

  void texture_2d::build_mipmaps(int base, int max_level)
  {
    MGL_CORE_ASSERT(!m_released, "Texture2D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(base <= max_level, "invalid base");

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(texture_target, m_texture_obj);

    glTexParameteri(texture_target, GL_TEXTURE_BASE_LEVEL, base);
    glTexParameteri(texture_target, GL_TEXTURE_MAX_LEVEL, max_level);

    glGenerateMipmap(texture_target);

    glTexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_filter = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR };
    m_max_level = max_level;
  }

  void texture_2d::set_repeat_x(bool value)
  {
    MGL_CORE_ASSERT(!m_released, "Texture2D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(texture_target, m_texture_obj);

    m_repeat_x = value;

    if(m_repeat_x)
    {
      glTexParameteri(texture_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
      return;
    }

    glTexParameteri(texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  }

  void texture_2d::set_repeat_y(bool value)
  {
    MGL_CORE_ASSERT(!m_released, "Texture2D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(texture_target, m_texture_obj);

    m_repeat_y = value;

    if(m_repeat_y)
    {
      glTexParameteri(texture_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
      return;
    }

    glTexParameteri(texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  void texture_2d::set_filter(const texture::filter& value)
  {
    MGL_CORE_ASSERT(!m_released, "Texture2D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    m_filter = value;

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(texture_target, m_texture_obj);
    glTexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, m_filter.min_filter);
    glTexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, m_filter.mag_filter);
  }

  std::string texture_2d::swizzle()
  {
    MGL_CORE_ASSERT(!m_released, "Texture2D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(!m_depth, "cannot get swizzle of depth textures");

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(texture_target, m_texture_obj);

    int swizzle_r = 0;
    int swizzle_g = 0;
    int swizzle_b = 0;
    int swizzle_a = 0;

    glGetTexParameteriv(texture_target, GL_TEXTURE_SWIZZLE_R, &swizzle_r);
    glGetTexParameteriv(texture_target, GL_TEXTURE_SWIZZLE_G, &swizzle_g);
    glGetTexParameteriv(texture_target, GL_TEXTURE_SWIZZLE_B, &swizzle_b);
    glGetTexParameteriv(texture_target, GL_TEXTURE_SWIZZLE_A, &swizzle_a);

    char swizzle[5] = {
      char_from_swizzle(swizzle_r),
      char_from_swizzle(swizzle_g),
      char_from_swizzle(swizzle_b),
      char_from_swizzle(swizzle_a),
      0,
    };

    return swizzle;
  }

  void texture_2d::set_swizzle(const std::string& value)
  {
    MGL_CORE_ASSERT(!m_released, "Texture2D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(!m_depth, "cannot set swizzle for depth textures");

    const char* swizzle = value.c_str();
    MGL_CORE_ASSERT(swizzle[0], "the swizzle is empty");

    int tex_swizzle[4] = { -1, -1, -1, -1 };

    for(int i = 0; swizzle[i]; ++i)
    {
      MGL_CORE_ASSERT(i < 4, "the swizzle is too long");
      tex_swizzle[i] = swizzle_from_char(swizzle[i]);
      MGL_CORE_ASSERT(tex_swizzle[i] != -1, "'{0}' is not a valid swizzle parameter", swizzle[i]);
    }

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(texture_target, m_texture_obj);

    glTexParameteri(texture_target, GL_TEXTURE_SWIZZLE_R, tex_swizzle[0]);
    if(tex_swizzle[1] != -1)
    {
      glTexParameteri(texture_target, GL_TEXTURE_SWIZZLE_G, tex_swizzle[1]);
      if(tex_swizzle[2] != -1)
      {
        glTexParameteri(texture_target, GL_TEXTURE_SWIZZLE_B, tex_swizzle[2]);
        if(tex_swizzle[3] != -1)
        {
          glTexParameteri(texture_target, GL_TEXTURE_SWIZZLE_A, tex_swizzle[3]);
        }
      }
    }
  }

  void texture_2d::set_compare_func(mgl::opengl::compare_func value)
  {
    MGL_CORE_ASSERT(!m_released, "Texture2D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(m_depth, "only depth textures have compare_func");

    m_compare_func = value;

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(texture_target, m_texture_obj);

    if(m_compare_func == 0)
    {
      glTexParameteri(texture_target, GL_TEXTURE_COMPARE_MODE, GL_NONE);
    }
    else
    {
      glTexParameteri(texture_target, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
      glTexParameteri(texture_target, GL_TEXTURE_COMPARE_FUNC, m_compare_func);
    }
  }

  void texture_2d::set_anisotropy(float value)
  {
    MGL_CORE_ASSERT(!m_released, "Texture2D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    m_anisotropy = (float)MGL_MIN(MGL_MAX(value, 1.0), m_context->max_anisotropy());
    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(texture_target, m_texture_obj);

    glTexParameterf(texture_target, GL_TEXTURE_MAX_ANISOTROPY, m_anisotropy);
  }

} // namespace  mgl::opengl
