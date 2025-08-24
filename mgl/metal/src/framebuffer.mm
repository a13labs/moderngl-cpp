// Minimal Metal framebuffer implementation (placeholder to satisfy linker)
#import <Foundation/Foundation.h>

#include "mgl_core/debug.hpp"
#include "mgl_metal/framebuffer.hpp"

namespace mgl::metal
{
  framebuffer::framebuffer(const context_ref& ctx)
      : object(ctx)
      , m_viewport({ 0, 0, 0, 0 })
      , m_scissor_enabled(false)
      , m_scissor({ 0, 0, 0, 0 })
      , m_color_masks()
      , m_draw_buffers()
      , m_dynamic(true)
      , m_width(0)
      , m_height(0)
      , m_samples(0)
      , m_depth_mask(false)
  {
    // default framebuffer placeholder
    m_viewport = { 0, 0, 0, 0 };
    m_scissor = m_viewport;
  }

  framebuffer::framebuffer(const context_ref& ctx,
                           const attachments_ref& color_attachments,
                           attachment_ref depth_attachment)
      : object(ctx)
      , m_viewport({ 0, 0, 0, 0 })
      , m_scissor_enabled(false)
      , m_scissor({ 0, 0, 0, 0 })
      , m_color_masks()
      , m_draw_buffers()
      , m_dynamic(false)
      , m_width(0)
      , m_height(0)
      , m_samples(0)
      , m_depth_mask(false)
  {
    // Construct a simple framebuffer descriptor using attachments information when
    // available. This is a minimal placeholder implementation sufficient for
    // platform wrapper usage in the higher-level code.
    if(color_attachments.size())
    {
      auto first = color_attachments[0];
      m_width = first->width();
      m_height = first->height();
      m_samples = first->samples();
      m_color_masks.reserve(color_attachments.size());
      for(auto&& a : color_attachments)
      {
        m_color_masks.push_back({ a->components() >= 1,
                                  a->components() >= 2,
                                  a->components() >= 3,
                                  a->components() >= 4 });
      }
    }

    if(depth_attachment)
    {
      m_depth_mask = true;
      if(m_width == 0)
      {
        m_width = depth_attachment->width();
        m_height = depth_attachment->height();
        m_samples = depth_attachment->samples();
      }
    }

    m_viewport = { 0, 0, m_width, m_height };
    m_scissor = m_viewport;
  }

  void framebuffer::release()
  {
    // Minimal release: mark as released by clearing handler
    m_handler = nullptr;
  }

  void
  framebuffer::clear(float r, float g, float b, float a, float depth, const mgl::rect& viewport)
  {
    // Placeholder clear; simply update viewport/flags. Real Metal clear would
    // encode commands on a render encoder.
    if(viewport != mgl::null_viewport_2d)
    {
      m_viewport = viewport;
    }
  }

  void framebuffer::set_color_mask(const metal::color_mask& mask)
  {
    m_color_masks.clear();
    m_color_masks.push_back(mask);
  }

  void framebuffer::set_color_mask(const metal::color_masks& masks)
  {
    m_color_masks = masks;
  }

  void framebuffer::set_depth_mask(bool value)
  {
    m_depth_mask = value;
  }

  void framebuffer::set_viewport(const mgl::rect& r)
  {
    m_viewport = r;
  }

  void framebuffer::set_scissor(const mgl::rect& r)
  {
    m_scissor = r;
  }

  void framebuffer::use()
  {
    // Placeholder: do nothing. Real implementation should bind the framebuffer
    // into a Metal render pass / encoder. Keeping this a no-op prevents
    // accidental access to private context members in this simplified stub.
  }

} // namespace mgl::metal
