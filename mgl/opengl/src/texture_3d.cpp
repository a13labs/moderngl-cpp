
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
#include "mgl_opengl/texture_3d.hpp"
#include "mgl_opengl/buffer.hpp"
#include "mgl_opengl/context.hpp"
#include "mgl_opengl/data_type.hpp"

#include "mgl_opengl_internal/utils.hpp"

#include "mgl_core/debug.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  texture_3d::texture_3d(const context_ref& ctx,
                         int32_t w,
                         int32_t h,
                         int32_t depth,
                         int32_t components,
                         const void* data,
                         int32_t align,
                         const std::string& dtype)
      : gl_object(ctx)
  {
    MGL_CORE_ASSERT(w > 0, "width must be greater than 0");
    MGL_CORE_ASSERT(h > 0, "height must be greater than 0");
    MGL_CORE_ASSERT(depth > 0, "depth must be greater than 0");
    MGL_CORE_ASSERT(components > 0 && components < 5, "components must be 1, 2, 3 or 4");

    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "align must be 1, 2, 4 or 8");

    auto data_type = from_dtype(dtype);

    if(!data_type)
    {
      MGL_CORE_ASSERT(false, "Invalid data type got: '{0}'", dtype);
      return;
    }

    m_width = w;
    m_height = h;
    m_depth = depth;
    m_components = components;
    m_data_type = data_type;
    m_max_lvl = 0;

    auto filter = data_type->float_type ? GL_LINEAR : GL_NEAREST;
    m_filter = { filter, filter };

    m_repeat_x = true;
    m_repeat_y = true;
    m_repeat_z = true;

    GLuint glo = 0;

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->max_texture_units());
    glGenTextures(1, &glo);

    if(!glo)
    {
      MGL_CORE_ASSERT(false, "cannot create texture");
      return;
    }

    gl_object::set_glo(glo);

    int32_t pixel_type = data_type->gl_type;
    int32_t base_format = data_type->base_format[components];
    int32_t internal_format = data_type->internal_format[components];

    glBindTexture(GL_TEXTURE_3D, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glTexImage3D(GL_TEXTURE_3D, 0, internal_format, w, h, depth, 0, base_format, pixel_type, data);

    if(data_type->float_type)
    {
      glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
      glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
  }

  void texture_3d::release()
  {
    MGL_CORE_ASSERT(!gl_object::released(), "texture already released");
    GLuint glo = gl_object::glo();
    glDeleteTextures(1, &glo);
    gl_object::set_glo(GL_ZERO);
  }

  void texture_3d::read(mgl::uint8_buffer& dst, int32_t align, size_t dst_offset)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "texture already released");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "align must be 1, 2, 4 or 8");

    size_t expected_size = m_width * m_components * m_data_type->size;
    expected_size = (expected_size + align - 1) / align * align;
    expected_size = expected_size * m_height * m_depth;
    MGL_CORE_ASSERT(dst.size() >= dst_offset + expected_size, "out of bounds");

    int32_t pixel_type = m_data_type->gl_type;
    int32_t base_format = m_data_type->base_format[m_components];

    char* ptr = (char*)dst.data() + dst_offset;

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glGetTexImage(GL_TEXTURE_3D, 0, base_format, pixel_type, ptr);

    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "OpenGL error");
  }

  void texture_3d::read(buffer_ref& dst, int32_t align, size_t dst_offset)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "texture already released");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "align must be 1, 2, 4 or 8");

    int32_t pixel_type = m_data_type->gl_type;
    int32_t base_format = m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_PACK_BUFFER, dst->glo());
    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glGetTexImage(GL_TEXTURE_3D, 0, base_format, pixel_type, (void*)dst_offset);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "OpenGL error");
  }

  void texture_3d::write(const mgl::uint8_buffer& src, const mgl::cube& v, int32_t align)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "texture already released");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "align must be 1, 2, 4 or 8");

    int32_t x = v.x;
    int32_t y = v.y;
    int32_t z = v.z;
    int32_t width = v.width;
    int32_t height = v.height;
    int32_t depth = v.depth;

    size_t expected_size = width * m_components * m_data_type->size;
    expected_size = (expected_size + align - 1) / align * align;
    expected_size = expected_size * height * depth;
    MGL_CORE_ASSERT(src.size() >= expected_size, "out of bounds");

    int32_t base_format = m_data_type->base_format[m_components];
    int32_t pixel_type = m_data_type->gl_type;

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glTexSubImage3D(
        GL_TEXTURE_3D, 0, x, y, z, width, height, depth, base_format, pixel_type, src.data());

    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "OpenGL error");
  }

  void texture_3d::write(const mgl::uint8_buffer& src, int32_t align)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "texture already released");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "align must be 1, 2, 4 or 8");

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    int32_t width = m_width;
    int32_t height = m_height;
    int32_t depth = m_depth;

    size_t expected_size = width * m_components * m_data_type->size;
    expected_size = (expected_size + align - 1) / align * align;
    expected_size = expected_size * height * depth;
    MGL_CORE_ASSERT(src.size() >= expected_size, "out of bounds");

    int32_t pixel_type = m_data_type->gl_type;
    int32_t base_format = m_data_type->base_format[m_components];

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glTexSubImage3D(
        GL_TEXTURE_3D, 0, x, y, z, width, height, depth, base_format, pixel_type, src.data());

    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "OpenGL error");
  }

  void texture_3d::write(const buffer_ref& src, const mgl::cube& v, int32_t align)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "texture already released");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "align must be 1, 2, 4 or 8");

    int32_t x = v.x;
    int32_t y = v.y;
    int32_t z = v.z;
    int32_t width = v.width;
    int32_t height = v.height;
    int32_t depth = v.depth;

    int32_t pixel_type = m_data_type->gl_type;
    int32_t base_format = m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, src->glo());
    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glTexSubImage3D(GL_TEXTURE_3D, 0, x, y, z, width, height, depth, base_format, pixel_type, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "OpenGL error");
  }

  void texture_3d::write(const buffer_ref& src, int32_t align)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "texture already released");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "align must be 1, 2, 4 or 8");

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    int32_t width = m_width;
    int32_t height = m_height;
    int32_t depth = m_depth;

    int32_t pixel_type = m_data_type->gl_type;
    int32_t base_format = m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, src->glo());
    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glTexSubImage3D(GL_TEXTURE_3D, 0, x, y, z, width, height, depth, base_format, pixel_type, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "OpenGL error");
  }

  void texture_3d::bind_to_image(int32_t unit, bool read, bool write, int32_t level, int32_t format)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "texture already released");
    MGL_CORE_ASSERT(read || write, "Illegal access mode. Read or write needs to be enabled.");

    int32_t access = GL_READ_WRITE;
    if(read && !write)
      access = GL_READ_ONLY;
    else if(!read && write)
      access = GL_WRITE_ONLY;

    int32_t frmt = format ? format : m_data_type->internal_format[m_components];

    glBindImageTexture(unit, gl_object::glo(), level, GL_TRUE, 0, access, frmt);
  }

  void texture_3d::use(int32_t index)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "texture already released");

    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_3D, gl_object::glo());
  }

  void texture_3d::build_mipmaps(int32_t base, int32_t max_lvl)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "texture already released");
    MGL_CORE_ASSERT(base <= max_lvl, "invalid base");

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, gl_object::glo());

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, base);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, m_max_lvl);

    glGenerateMipmap(GL_TEXTURE_3D);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_filter = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR };
    m_max_lvl = max_lvl;
  }

  void texture_3d::set_repeat_x(bool value)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "texture already released");

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, gl_object::glo());

    m_repeat_x = value;

    if(m_repeat_x)
    {
      glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      return;
    }

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  }

  void texture_3d::set_repeat_y(bool value)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "texture already released");

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, gl_object::glo());

    m_repeat_y = value;

    if(m_repeat_y)
    {
      glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      return;
    }

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  void texture_3d::set_repeat_z(bool value)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "texture already released");

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, gl_object::glo());

    m_repeat_z = value;

    if(m_repeat_z)
    {
      glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
      return;
    }

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  }

  void texture_3d::set_filter(const texture::filter& value)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "texture already released");

    m_filter = value;

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, gl_object::glo());

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, m_filter.min_filter);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, m_filter.mag_filter);
  }

  std::string texture_3d::swizzle()
  {
    MGL_CORE_ASSERT(!gl_object::released(), "texture already released");

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, gl_object::glo());

    int32_t swizzle_r = 0;
    int32_t swizzle_g = 0;
    int32_t swizzle_b = 0;
    int32_t swizzle_a = 0;

    glGetTexParameteriv(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_R, &swizzle_r);
    glGetTexParameteriv(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_G, &swizzle_g);
    glGetTexParameteriv(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_B, &swizzle_b);
    glGetTexParameteriv(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_A, &swizzle_a);

    char swizzle[5] = {
      internal::char_from_swizzle(swizzle_r),
      internal::char_from_swizzle(swizzle_g),
      internal::char_from_swizzle(swizzle_b),
      internal::char_from_swizzle(swizzle_a),
      0,
    };

    return swizzle;
  }

  void texture_3d::set_swizzle(const std::string& value)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "texture already released");

    const char* swizzle = value.c_str();
    MGL_CORE_ASSERT(swizzle[0], "the swizzle is empty");

    int32_t tex_swizzle[4] = { -1, -1, -1, -1 };

    for(int32_t i = 0; swizzle[i]; ++i)
    {
      MGL_CORE_ASSERT(i < 4, "the swizzle is too long");
      tex_swizzle[i] = internal::swizzle_from_char(swizzle[i]);
      MGL_CORE_ASSERT(tex_swizzle[i] != -1, "'{0}' is not a valid swizzle parameter", swizzle[i]);
    }

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, gl_object::glo());

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_R, tex_swizzle[0]);
    if(tex_swizzle[1] != -1)
    {
      glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_G, tex_swizzle[1]);
      if(tex_swizzle[2] != -1)
      {
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_B, tex_swizzle[2]);
        if(tex_swizzle[3] != -1)
        {
          glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_A, tex_swizzle[3]);
        }
      }
    }
  }
} // namespace  mgl::opengl
