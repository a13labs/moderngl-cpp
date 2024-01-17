#pragma once

#include "mgl_graphics/texture.hpp"
#include "mgl_registry/resources/image.hpp"

namespace mgl::graphics
{
  class texture2d : public texture
  {
public:
    texture2d(const mgl::registry::image_ref& image, const texture_opts& opts = {});
    ~texture2d() = default;

    virtual texture::type texture_type() override final;
    virtual const mgl::window::api::texture_ref& native() override final;
    virtual void prepare() override final;
    virtual void load() override final;
    virtual void unload() override final;

private:
    mgl::registry::image_ref m_image;
    texture_opts m_opts;
    mgl::window::api::texture_ref m_texture;
  };
} // namespace mgl::graphics