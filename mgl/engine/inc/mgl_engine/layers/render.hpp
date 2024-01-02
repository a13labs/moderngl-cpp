#pragma once

#include "mgl_engine/layer.hpp"

namespace mgl::engine::layers
{
  class render_layer : public layer
  {
public:
    render_layer(const std::string& name = "Renderer Layer");

    virtual ~render_layer() override = default;

    virtual void on_attach() override;

    virtual void on_detach() override;

    virtual void on_update(float time, float frame_time) override final;

    virtual void render_prepare() = 0;

    void on_event(mgl::window::event& event) override;
  };

  class null_render_layer : public render_layer
  {
public:
    null_render_layer(const std::string& name = "Null Renderer Layer")
        : render_layer(name)
    { }

    virtual ~null_render_layer() override = default;

    virtual void render_prepare() override { }
  };

} // namespace mgl::engine::layers