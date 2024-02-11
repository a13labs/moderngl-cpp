#include "mgl_graphics/textures/texture2d.hpp"

#include "mgl_core/debug.hpp"

#include "mgl_platform/api/render_api.hpp"

namespace mgl::graphics
{

  texture::type texture2d::texture_type()
  {
    return texture::type::TEXTURE_2D;
  }

  void texture2d::prepare() { }

  void texture2d::load()
  {
    MGL_CORE_ASSERT(m_texture == nullptr, "Texture already loaded");
    MGL_CORE_ASSERT(m_image != nullptr, "Image is null");
    m_texture = mgl::platform::api::render_api::create_texture_2d(m_image, m_opts.samples);
    MGL_CORE_ASSERT(m_texture != nullptr, "Texture is null");
    // tex->set_filter({ (int)m_opts.min_filter, (int)m_opts.mag_filter });
  }

  void texture2d::bind(int index)
  {
    MGL_CORE_ASSERT(m_texture != nullptr, "Texture is null");
    m_texture->bind(index);
  }

  void texture2d::unload()
  {
    MGL_CORE_ASSERT(m_texture != nullptr, "Texture is null");
    m_texture->release();
  }

} // namespace mgl::graphics