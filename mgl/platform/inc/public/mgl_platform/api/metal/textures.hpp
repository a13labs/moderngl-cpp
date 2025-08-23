#pragma once

#include "mgl_platform/api/textures.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/memory.hpp"

#include "mgl_metal/texture_2d.hpp"
#include "mgl_metal/texture_3d.hpp"
#include "mgl_metal/texture_cube.hpp"

namespace mgl::platform::api::backends
{
  class metal_texture_2d;
  using metal_texture_2d_ref = mgl::ref<metal_texture_2d>;

  class metal_texture_2d : public mgl::platform::api::texture_2d
  {
public:
    metal_texture_2d(const mgl::size& size, int32_t components, int32_t samples);

    virtual ~metal_texture_2d() = default;

    virtual void release() override final { m_texture->release(); }

    virtual void bind(int32_t slot) override final { m_texture->use(slot); }

    virtual const texture::filter& get_filter() const override final;

    virtual void set_filter(const texture::filter& value) override final;

    virtual const mgl::size size() const override final
    {
      return { m_texture->width(), m_texture->height() };
    }

    virtual int32_t components() const override final { return m_texture->components(); }

    virtual int32_t samples() const override final { return m_texture->samples(); }

    virtual bool depth() const override final { return m_texture->depth(); }

    virtual void upload(const uint8_buffer& src,
                        const mgl::rect& v,
                        int32_t lvl = 0,
                        int32_t align = 1) override final
    {
      m_texture->write(src, v, lvl, align);
    }

    mgl::metal::texture_2d_ref& native() { return m_texture; }

private:
    friend class metal_api;
    mgl::metal::texture_2d_ref m_texture;
  };
} // namespace mgl::platform::api::backends
