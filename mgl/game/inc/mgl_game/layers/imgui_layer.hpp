#pragma once

#include "mgl_game/layer.hpp"

#include "mgl_game/integrations/imgui.hpp"

namespace mgl::game::layers
{
  class imgui_layer : public layer
  {
public:
    imgui_layer();
    ~imgui_layer() override;

    void on_attach() override;
    void on_detach() override;

    void on_draw(float time, float frame_time) override;
    bool on_event(event& event) override;
  };
} // namespace mgl::game::layers