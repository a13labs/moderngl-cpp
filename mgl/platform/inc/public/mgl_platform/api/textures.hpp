#pragma once

#include "mgl_core/debug.hpp"
#include "mgl_core/math.hpp"
#include "mgl_core/memory.hpp"

#include "mgl_registry/resources/image.hpp"

namespace mgl::platform::api
{

  class texture;
  using texture_ref = mgl::ref<texture>;

  class texture_2d;
  using texture_2d_ref = mgl::ref<texture_2d>;

  class texture
  {
public:
    enum class type
    {
      TEXTURE_2D,
      TEXTURE_3D,
      TEXTURE_CUBE,
    };

    enum class filter : int32_t
    {
      NEAREST,
      LINEAR,
      NEAREST_MIPMAP_NEAREST,
      LINEAR_MIPMAP_NEAREST,
      NEAREST_MIPMAP_LINEAR,
      LINEAR_MIPMAP_LINEAR,
    };

    ~texture() = default;

    virtual void release() = 0;

    virtual void bind(int32_t slot) = 0;

    virtual texture::type texture_type() = 0;

    virtual const texture::filter& get_filter() const = 0;

    virtual void set_filter(const texture::filter& value) = 0;
  };

  class texture_2d : public texture
  {
public:
    virtual ~texture_2d() = default;

    virtual texture::type texture_type() override { return texture::type::TEXTURE_2D; }

    virtual const mgl::size size() const = 0;

    virtual int32_t components() const = 0;

    virtual int32_t samples() const = 0;

    virtual bool depth() const = 0;

    virtual void
    upload(const uint8_buffer& src, const mgl::rect& v, int32_t lvl = 0, int32_t align = 1) = 0;

    void upload(const mgl::registry::image_ref& src,
                const mgl::rect& v,
                int32_t lvl = 0,
                int32_t align = 1)
    {
      MGL_CORE_ASSERT(src->channels() == components(),
                      "Image components do not match texture components");
      upload(src->buffer(), v, lvl, align);
    }

    void upload(const mgl::registry::image_ref& src, int32_t lvl = 0, int32_t align = 1)
    {
      MGL_CORE_ASSERT(src->channels() == components(),
                      "Image components do not match texture components");
      upload(src, { 0, 0, src->width(), src->height() }, lvl, align);
    }
  };

  class texture_3d : public texture
  {
public:
    virtual ~texture_3d() = default;

    virtual texture::type texture_type() override { return texture::type::TEXTURE_3D; }

    virtual const mgl::size size() const = 0;

    virtual int32_t components() const = 0;

    virtual int32_t samples() const = 0;

    virtual int32_t depth() const = 0;
  };

  class texture_cube : public texture
  {
public:
    virtual ~texture_cube() = default;

    virtual texture::type texture_type() override { return texture::type::TEXTURE_CUBE; }

    virtual const mgl::size size() const = 0;

    virtual int32_t components() const = 0;
  };
} // namespace mgl::platform::api