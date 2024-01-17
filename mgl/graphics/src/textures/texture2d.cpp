#include "mgl_graphics/textures/texture2d.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_graphics/render.hpp"

namespace mgl::graphics
{

  texture2d::texture2d(const mgl::registry::image_ref& image, const texture_opts& opts)
      : m_texture(nullptr)
      , m_image(image)
      , m_opts(opts)
  { }

  texture::type texture2d::texture_type()
  {
    return texture::type::TEXTURE_2D;
  }

  const mgl::window::api::texture_ref& texture2d::native()
  {
    return m_texture;
  }

  void texture2d::prepare() { }

  void texture2d::load()
  {
    MGL_CORE_ASSERT(m_texture == nullptr, "Texture already loaded");
    MGL_CORE_ASSERT(m_image != nullptr, "Image is null");
    auto& render = mgl::graphics::current_render();
    auto ctx = render.context();

    auto tex =
        ctx->texture2d(m_image->width(), m_image->height(), m_image->channels(), m_image->data());

    tex->set_filter({ (int)m_opts.min_filter, (int)m_opts.mag_filter });
    m_texture = tex;
  }

  void texture2d::unload()
  {
    MGL_CORE_ASSERT(m_texture != nullptr, "Texture is null");
    m_texture->release();
    m_texture = nullptr;
  }
} // namespace mgl::graphics