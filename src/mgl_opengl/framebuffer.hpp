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
#pragma once
#include "builtins.hpp"
#include "glm/glm.hpp"

namespace mgl::opengl
{
  class framebuffer : public mgl::core::ref_from_this<framebuffer>
  {
public:
    ~framebuffer() = default;

    void release();
    bool released();

    const mgl::core::viewport_2d& viewport();
    void set_viewport(const mgl::core::viewport_2d& r);

    const mgl::core::viewport_2d& scissor();
    void set_scissor(const mgl::core::viewport_2d& r);

    void enable_scissor();
    void disable_scissor();

    const color_masks& color_mask() const;
    void set_color_mask(const mgl::opengl::color_mask& mask);
    void set_color_mask(const mgl::opengl::color_masks& masks);

    bool depth_mask();
    void set_depth_mask(bool value);

    int width();
    int height();

    bool bits(int& red_bits, int& green_bits, int& blue_bits, int& alpha_bits, int& depth_bits, int& stencil_bits);

    void clear(const glm::vec4& color, float depth = 0.0, const mgl::core::viewport_2d& viewport = mgl::core::null_viewport_2d);
    void clear(float r,
               float g,
               float b,
               float a = 0.0,
               float depth = 0.0,
               const mgl::core::viewport_2d& viewport = mgl::core::null_viewport_2d);

    bool read_into(mgl::core::mem_buffer<u_int8_t>& dst,
                   const mgl::core::viewport_2d& viewport = mgl::core::null_viewport_2d,
                   int components = 3,
                   int attachment = 0,
                   int alignment = 1,
                   const char* dtype = "f1",
                   size_t write_offset = 0);

    bool read_into(buffer_ref dst,
                   const mgl::core::viewport_2d& viewport = mgl::core::null_viewport_2d,
                   int components = 3,
                   int attachment = 0,
                   int alignment = 1,
                   const char* dtype = "f1",
                   size_t write_offset = 0);

    void use();

    int glo();

private:
    friend class context;
    friend class renderbuffer;
    friend class texture_2d;

    framebuffer() = default;

    context* m_context;
    int m_framebuffer_obj;

    mgl::core::viewport_2d m_viewport;
    bool m_scissor_enabled;
    mgl::core::viewport_2d m_scissor;
    color_masks m_color_masks;

    int m_draw_buffers_len;
    unsigned* m_draw_buffers;

    // Flags this as a detected framebuffer we don't control the size of
    bool m_dynamic;
    int m_width;
    int m_height;

    int m_samples;
    bool m_depth_mask;
    bool m_released;
  };

  inline int framebuffer::glo()
  {
    return m_framebuffer_obj;
  }

  inline bool framebuffer::released()
  {
    return m_released;
  }

  inline const mgl::core::viewport_2d& framebuffer::viewport()
  {
    return m_viewport;
  }

  inline const mgl::core::viewport_2d& framebuffer::scissor()
  {
    return m_scissor;
  }

  inline void framebuffer::enable_scissor()
  {
    m_scissor_enabled = true;
  }
  inline void framebuffer::disable_scissor()
  {
    m_scissor_enabled = false;
  }

  inline void framebuffer::clear(const glm::vec4& color, float depth, const mgl::core::viewport_2d& viewport)
  {
    clear(color.r, color.g, color.b, color.a, depth, viewport);
  }

  inline const color_masks& framebuffer::color_mask() const
  {
    return m_color_masks;
  }

  inline bool framebuffer::depth_mask()
  {
    return m_depth_mask;
  }

  inline int framebuffer::width()
  {
    return m_width;
  }

  inline int framebuffer::height()
  {
    return m_height;
  }

  inline void framebuffer::set_color_mask(const mgl::opengl::color_mask& mask)
  {
    set_color_mask({ mask });
  }

} // namespace  mgl::opengl