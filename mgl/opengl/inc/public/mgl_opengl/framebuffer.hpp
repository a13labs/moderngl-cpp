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
#include "buffer.hpp"
#include "color_mask.hpp"
#include "gl_object.hpp"

#include "mgl_core/math.hpp"
#include "mgl_core/memory.hpp"

#include "glm/vec4.hpp"

namespace mgl::opengl
{
  class context;

  class framebuffer : public gl_object
  {
public:
    ~framebuffer() = default;

    bool released() const { return m_glo == 0; }

    const mgl::rect& viewport() const { return m_viewport; }

    const mgl::rect& scissor() const { return m_scissor; }

    void enable_scissor() { m_scissor_enabled = true; }

    void disable_scissor() { m_scissor_enabled = false; }

    void clear(const glm::vec4& color, float depth, const mgl::rect& viewport)
    {
      clear(color.r, color.g, color.b, color.a, depth, viewport);
    }

    const color_masks& color_mask() const { return m_color_masks; }

    bool depth_mask() { return m_depth_mask; }

    int32_t width() { return m_width; }

    int32_t height() { return m_height; }

    void set_color_mask(const opengl::color_mask& mask) { set_color_mask({ mask }); }

    void release();
    void set_viewport(const mgl::rect& r);
    void set_scissor(const mgl::rect& r);
    void reset_scissor() { set_scissor(m_viewport); }
    void set_color_mask(const opengl::color_masks& masks);
    void set_depth_mask(bool value);

    void bits(int32_t& red_bits,
              int32_t& green_bits,
              int32_t& blue_bits,
              int32_t& alpha_bits,
              int32_t& depth_bits,
              int32_t& stencil_bits);

    void clear(const glm::vec4& color,
               float depth = 0.0,
               const mgl::rect& viewport = mgl::null_viewport_2d);

    void clear(float r,
               float g,
               float b,
               float a = 0.0,
               float depth = 0.0,
               const mgl::rect& viewport = mgl::null_viewport_2d);

    void read(mgl::uint8_buffer& dst,
              const mgl::rect& viewport = mgl::null_viewport_2d,
              int32_t components = 3,
              int32_t attachment = 0,
              int32_t alignment = 1,
              const char* dtype = "f1",
              size_t write_offset = 0);

    void read(buffer_ref dst,
              const mgl::rect& viewport = mgl::null_viewport_2d,
              int32_t components = 3,
              int32_t attachment = 0,
              int32_t alignment = 1,
              const char* dtype = "f1",
              size_t write_offset = 0);

    void use();

private:
    friend class context;

    framebuffer(context* ctx);
    framebuffer(context* ctx,
                const attachments_ref& color_attachments,
                attachment_ref depth_attachment);

    mgl::rect m_viewport;
    bool m_scissor_enabled;
    mgl::rect m_scissor;
    color_masks m_color_masks;

    int32_t m_draw_buffers_len;
    unsigned* m_draw_buffers;

    // Flags this as a detected framebuffer we don't control the size of
    bool m_dynamic;
    int32_t m_width;
    int32_t m_height;

    int32_t m_samples;
    bool m_depth_mask;
  };

  using framebuffer_ref = mgl::ref<mgl::opengl::framebuffer>;
  using framebuffer_weak_ref = mgl::weak_ref<mgl::opengl::framebuffer>;

} // namespace  mgl::opengl