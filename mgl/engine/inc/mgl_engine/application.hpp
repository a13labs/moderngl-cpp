#pragma once

#include "layer.hpp"
#include "renderer.hpp"

#include "mgl_window/window.hpp"

namespace mgl::engine
{

  class application : public mgl::window::window
  {
public:
    application(const mgl::window::window_config& settings = mgl::window::window_config());
    ~application() override;

    layer_stack& layers() { return m_layers; }

    const renderer_ref& current_renderer() const;

    void on_event(mgl::window::event& event) override;
    void on_draw(float time, float frame_time) override;
    bool on_load() override;
    void on_unload() override;

private:
    renderer_ref m_renderer;
    layer_stack m_layers;
  };

} // namespace mgl::engine
