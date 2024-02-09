#pragma once
#include "attachment.hpp"
#include "buffer.hpp"
#include "color_mask.hpp"
#include "gl_object.hpp"

#include "mgl_core/math.hpp"
#include "mgl_core/memory.hpp"

#include "glm/vec4.hpp"

namespace mgl::opengl
{
  class framebuffer : public gl_object, public mgl::ref_from_this<framebuffer>
  {
public:
    ~framebuffer() = default;

    virtual void release() override;

    const mgl::rect& viewport() const { return m_viewport; }

    const mgl::rect& scissor() const { return m_scissor; }

    void enable_scissor() { m_scissor_enabled = true; }

    void disable_scissor() { m_scissor_enabled = false; }

    void clear(const glm::vec4& color, float depth, const mgl::rect& viewport)
    {
      clear(color.r, color.g, color.b, color.a, depth, viewport);
    }

    void clear(float r,
               float g,
               float b,
               float a = 0.0,
               float depth = 0.0,
               const mgl::rect& viewport = mgl::null_viewport_2d);

    void clear() { clear(0, 0, 0, 0, 0, m_viewport); }

    const color_masks& color_mask() const { return m_color_masks; }

    bool depth_mask() { return m_depth_mask; }

    int32_t width() { return m_width; }

    int32_t height() { return m_height; }

    int32_t samples() { return m_samples; }

    void set_color_mask(const opengl::color_mask& mask);

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

    framebuffer(const context_ref& ctx);
    framebuffer(const context_ref& ctx,
                const attachments_ref& color_attachments,
                attachment_ref depth_attachment);

    mgl::rect m_viewport;
    bool m_scissor_enabled;
    mgl::rect m_scissor;
    color_masks m_color_masks;

    mgl::list<uint32_t> m_draw_buffers;

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