#pragma once

#include "mgl_game/layer.hpp"

#include "mgl_game/integrations/imgui.hpp"

namespace mgl::game::layers
{
  class imgui_layer : public layer
  {
public:
    imgui_layer(const std::string& name = "ImGui Layer")
        : layer(name)
    { }

    virtual ~imgui_layer() override = default;

    void on_attach() override
    {
      if(!mgl::game::imgui::is_initialized())
        mgl::game::imgui::init();
    };

    virtual void on_detach() override{};

    virtual void on_draw(float time, float frame_time) override
    {
      draw_ui(time, frame_time);
      mgl::game::imgui::render();
    };

    virtual void draw_ui(float time, float frame_time) = 0;

    void on_event(event& event) override { mgl::game::imgui::on_event(event); }
  };
} // namespace mgl::game::layers