#pragma once

#include "mgl_graphics/texture.hpp"
#include "mgl_registry/resources/image.hpp"

namespace mgl::graphics
{
  class texture2d : public texture
  {
public:
    texture2d(const mgl::registry::image_ref& image, const texture_opts& opts = {})
        : m_image(image)
        , m_opts(opts)
    { }
    ~texture2d() = default;

    virtual texture::type texture_type() override final;
    virtual void prepare() override final;
    virtual void load() override final;

private:
    mgl::registry::image_ref m_image;
    texture_opts m_opts;
  };

  using texture2d_ref = mgl::ref<texture2d>;
} // namespace mgl::graphics