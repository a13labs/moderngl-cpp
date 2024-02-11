#pragma once

#include "mgl_core/debug.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"

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

    virtual void bind(int index) = 0;

    virtual void unload() = 0;

    virtual texture::type texture_type() = 0;

    virtual void prepare() = 0;

    virtual void load() = 0;
  };

  struct texture_opts
  {
    bool generate_mipmaps = false;
    texture::filter min_filter = texture::filter::LINEAR;
    texture::filter mag_filter = texture::filter::LINEAR;
    int samples = 0;
  };

} // namespace mgl::graphics