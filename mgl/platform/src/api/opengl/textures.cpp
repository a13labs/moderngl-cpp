#include "mgl_platform/api/opengl/textures.hpp"
#include "mgl_platform/api/opengl/api.hpp"

#include "mgl_opengl/context.hpp"

#include "mgl_core/debug.hpp"

namespace mgl::platform::api::backends
{
  ogl_texture_2d::ogl_texture_2d(const mgl::size& size, int32_t components, int32_t samples)
  {
    mgl::opengl::context_ref& ctx = mgl::platform::api::backends::ogl_api::current_context();
    m_texture = ctx->texture2d(size.width, size.height, components, nullptr, samples);
  }

  const texture::filter& ogl_texture_2d::get_filter() const
  {
    MGL_CORE_ASSERT(false, "Not implemented");
    return texture::filter::NEAREST;
  }

  void ogl_texture_2d::set_filter(const texture::filter& value)
  {
    MGL_CORE_ASSERT(false, "Not implemented");
  }
} // namespace mgl::platform::api::backends
