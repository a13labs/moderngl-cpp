#pragma once

#include "mgl_core/debug.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"
#include "mgl_platform/api/opengl.hpp"

namespace mgl::graphics
{
  class texture;
  using texture_ref = mgl::ref<texture>;

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

    void bind(int index)
    {
      MGL_CORE_ASSERT(m_texture != nullptr, "Texture is not loaded")
      m_texture->use(index);
    }

    void unload()
    {
      MGL_CORE_ASSERT(m_texture != nullptr, "Texture is not loaded")
      m_texture->release();
    }

    const mgl::platform::api::texture_ref& api() { return m_texture; }

    virtual texture::type texture_type() = 0;
    virtual void prepare() = 0;
    virtual void load() = 0;

protected:
    mgl::platform::api::texture_ref m_texture = nullptr;
  };

  struct texture_opts
  {
    bool generate_mipmaps = false;
    texture::filter min_filter = texture::filter::LINEAR;
    texture::filter mag_filter = texture::filter::LINEAR;
    int samples = 0;
  };

} // namespace mgl::graphics