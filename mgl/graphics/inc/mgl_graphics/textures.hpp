#pragma once

#include "mgl_core/debug.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"

#include "mgl_platform/api/render_api.hpp"
#include "mgl_registry/resources/image.hpp"

namespace mgl::graphics
{
  class texture;
  using texture_ref = mgl::ref<texture>;

  class texture2d;
  using texture2d_ref = mgl::ref<texture2d>;

  class texture
  {
public:
    enum class type
    {
      TEXTURE_2D,
      TEXTURE_3D,
      TEXTURE_CUBE,
    };

    enum class filter : int
    {
      NEAREST = 0x2600,
      LINEAR = 0x2601,
      NEAREST_MIPMAP_NEAREST = 0x2700,
      LINEAR_MIPMAP_NEAREST = 0x2701,
      NEAREST_MIPMAP_LINEAR = 0x2702,
      LINEAR_MIPMAP_LINEAR = 0x2703,
    };

    ~texture() = default;

    virtual void unload() = 0;

    virtual texture::type texture_type() = 0;

    virtual void load() = 0;

    const mgl::platform::api::texture_ref& api() const { return m_texture; }

protected:
    mgl::platform::api::texture_ref m_texture;
  };

  struct texture_opts
  {
    bool generate_mipmaps = false;
    texture::filter min_filter = texture::filter::LINEAR;
    texture::filter mag_filter = texture::filter::LINEAR;
    int samples = 0;
  };

  class texture2d : public texture
  {
public:
    texture2d(const mgl::registry::image_ref& image, const texture_opts& opts = {})
        : m_image(image)
        , m_opts(opts)
    { }

    ~texture2d() = default;

    virtual texture::type texture_type() override final { return texture::type::TEXTURE_2D; }

    virtual void load() override final
    {
      MGL_CORE_ASSERT(m_texture == nullptr, "Texture already loaded");
      MGL_CORE_ASSERT(m_image != nullptr, "Image is null");
      m_texture = mgl::platform::api::render_api::create_texture_2d(m_image, m_opts.samples);
      MGL_CORE_ASSERT(m_texture != nullptr, "Texture is null");
      // tex->set_filter({ (int)m_opts.min_filter, (int)m_opts.mag_filter });
    }

    virtual void unload() override final
    {
      MGL_CORE_ASSERT(m_texture != nullptr, "Texture is null");
      m_texture->release();
    }

private:
    mgl::registry::image_ref m_image;
    texture_opts m_opts;
  };

} // namespace mgl::graphics