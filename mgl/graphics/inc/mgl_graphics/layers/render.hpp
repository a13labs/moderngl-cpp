#pragma once

#include "mgl_graphics/command.hpp"
#include "mgl_graphics/layer.hpp"

namespace mgl::graphics::layers
{
  class render_layer : public layer
  {
public:
    render_layer(const std::string& name = "Renderer Layer");

    virtual ~render_layer() override = default;

    virtual void on_attach() override;

    virtual void on_detach() override;

    virtual void on_update(float time, float frame_time) override final;

    virtual void render_prepare(render_script& script) = 0;

    void on_event(mgl::platform::event& event) override;
  };

  class null_render_layer : public render_layer
  {
public:
    null_render_layer(const std::string& name = "Null Renderer Layer")
        : render_layer(name)
    { }

    virtual ~null_render_layer() override = default;

    virtual void render_prepare(render_script& script) override { }
  };

} // namespace mgl::graphics::layers