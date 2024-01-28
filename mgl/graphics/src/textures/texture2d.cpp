#include "mgl_graphics/textures/texture2d.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_graphics/render.hpp"

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
    auto tex = mgl::window::api::create_texture_2d(m_image->width(),
                                                   m_image->height(),
                                                   m_image->channels(),
                                                   m_image->buffer(),
                                                   m_opts.samples);
    tex->set_filter({ (int)m_opts.min_filter, (int)m_opts.mag_filter });
    m_texture = tex;
  }

} // namespace mgl::graphics