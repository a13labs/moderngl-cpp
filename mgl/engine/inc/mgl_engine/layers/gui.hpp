#pragma once

#include "mgl_engine/layer.hpp"

#include "mgl_window/integrations/imgui.hpp"

namespace mgl::engine::layers
{
  class gui_layer : public layer
  {
public:
    gui_layer(const std::string& name = "ImGui Layer")
        : layer(name)
    { }

    virtual ~gui_layer() override = default;

    void on_attach() override
    {
      if(!mgl::window::imgui::is_initialized())
        mgl::window::imgui::init();
    };

    virtual void on_detach() override { }

    virtual void on_update(float time, float frame_time) override
    {
      draw_ui(time, frame_time);
      mgl::window::imgui::render();
    };

    virtual void draw_ui(float time, float frame_time) = 0;

    void on_event(mgl::window::event& event) override { mgl::window::imgui::on_event(event); }
  };

  class null_gui_layer : public gui_layer
  {
public:
    null_gui_layer(const std::string& name = "Null ImGui Layer")
        : gui_layer(name)
    { }

    virtual ~null_gui_layer() override = default;

    virtual void draw_ui(float time, float frame_time) override { }
  };
} // namespace mgl::engine::layers