#include "mgl_metal/context.hpp"
#include "mgl_metal/buffer.hpp"
#include "mgl_metal/program.hpp"
#include "mgl_metal/texture_2d.hpp"
#include "mgl_metal/texture_3d.hpp"
#include "mgl_metal/texture_cube.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/log.hpp"

#import <Metal/Metal.h>

namespace mgl::metal
{

  context_ref context::create_context(context_mode::mode mode)
  {
    return mgl::create_ref<context>(mode);
  }

  context::context(context_mode::mode mode)
  {
    id<MTLDevice> device = nil;

    switch(mode)
    {
      case context_mode::STANDALONE:
      case context_mode::DEFAULT: device = MTLCreateSystemDefaultDevice(); break;
      case context_mode::SHARE:
      case context_mode::ATTACHED:
        MGL_CORE_ASSERT(false, "[Metal Context] (share/attached) Mode not supported.")
        break;
    }

    if(!device)
    {
      MGL_CORE_ASSERT(false, "[Metal Context] Device not available");
    }

    m_device = (__bridge void*)device;
    m_commandQueue = (__bridge void*)[device newCommandQueue];
    m_mode = mode;
    m_released = false;
  }

  context::~context()
  {
    release();
  }

  void context::enter()
  {
    // Enter the Metal context
    if(m_device)
    {
      [(id<MTLDevice>)m_device retain];
    }
  }

  void context::exit()
  {
    // Exit the Metal context
    if(m_device)
    {
      [(id<MTLDevice>)m_device release];
    }
  }

  void context::release()
  {
    if(!m_released)
    {
      m_released = true;
      // Release Metal objects
      if(m_commandQueue)
      {
        m_commandQueue = nil;
      }
      if(m_device)
      {
        m_device = nil;
      }
    }
  }

  bool context::is_current() const
  {
    return m_device == MTLCreateSystemDefaultDevice();
  }

  void context::enable(int32_t flags)
  {
    m_enable_flags |= flags;
  }

  void context::disable(int32_t flags)
  {
    m_enable_flags &= ~flags;
  }

  void context::enable_direct(int32_t value)
  {
    m_enable_flags |= value;
  }

  void context::disable_direct(int32_t value)
  {
    m_enable_flags &= ~value;
  }

  void context::finish()
  {
    // No-op placeholder for Metal command queue completion
  }

  void context::clear_samplers(int32_t start, int32_t end)
  {
    // Placeholder: Metal sampler clearing would bind null samplers to the range
  }

  void context::set_blend_equation(blend_equation_mode modeRGB, blend_equation_mode modeAlpha)
  {
    // Store blend equation selection in enable flags as minimal placeholder
    (void)modeRGB;
    (void)modeAlpha;
  }

  void context::set_blend_func(blend_factor srcRGB,
                               blend_factor dstRGB,
                               blend_factor srcAlpha,
                               blend_factor dstAlpha)
  {
    m_blend_func_src = static_cast<int>(srcRGB);
    m_blend_func_dst = static_cast<int>(dstRGB);
  }

  void context::clear(const glm::vec4& color, float depth, const mgl::rect& viewport)
  {
    // Placeholder: actual Metal clear requires encoder commands.
    if(m_bound_framebuffer)
    {
      m_bound_framebuffer->clear(color, depth, viewport);
    }
  }

  buffer_ref context::buffer(void* data, size_t reserve, bool dynamic)
  {
    MGL_CORE_ASSERT(!released(), "[Metal Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[Metal Context] Resource context not current.");
    auto buffer = new mgl::metal::buffer(shared_from_this(), data, reserve, dynamic);
    return buffer_ref(buffer);
  }

  texture_2d_ref context::texture2d(int32_t width,
                                    int32_t height,
                                    int32_t components,
                                    const void* data,
                                    int32_t samples,
                                    int32_t alignment,
                                    const std::string& dtype,
                                    int32_t internal_format_override)
  {
    MGL_CORE_ASSERT(!released(), "[Metal Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[Metal Context] Resource context not current.");
    auto texture = new mgl::metal::texture_2d(shared_from_this(),
                                              width,
                                              height,
                                              components,
                                              data,
                                              samples,
                                              alignment,
                                              dtype,
                                              internal_format_override);
    return texture_2d_ref(texture);
  }

  texture_3d_ref context::texture3d(int32_t width,
                                    int32_t height,
                                    int32_t depth,
                                    int32_t components,
                                    const void* data,
                                    int32_t align,
                                    const std::string& dtype)
  {
    MGL_CORE_ASSERT(!released(), "[Metal Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[Metal Context] Resource context not current.");
    auto texture = new mgl::metal::texture_3d(
        shared_from_this(), width, height, depth, components, data, align, dtype);
    return texture_3d_ref(texture);
  }

  texture_cube_ref context::texture_cube(int32_t w,
                                         int32_t h,
                                         int32_t components,
                                         const void* data,
                                         int32_t align,
                                         const std::string& dtype,
                                         int32_t internal_format_override)
  {
    MGL_CORE_ASSERT(!released(), "[Metal Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[Metal Context] Resource context not current.");
    auto texture = new mgl::metal::texture_cube(
        shared_from_this(), w, h, components, data, align, dtype, internal_format_override);
    return texture_cube_ref(texture);
  }

  program_ref context::program(const shaders& shaders,
                               const shaders_outputs& outputs,
                               const fragment_outputs& fragment_outputs,
                               bool interleaved,
                               const std::string& filename)
  {
    MGL_CORE_ASSERT(!released(), "[Metal Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[Metal Context] Resource context not current.");
    auto prog = new mgl::metal::program(
        shared_from_this(), shaders, outputs, fragment_outputs, interleaved, filename);
    return program_ref(prog);
  }

} // namespace mgl::metal
