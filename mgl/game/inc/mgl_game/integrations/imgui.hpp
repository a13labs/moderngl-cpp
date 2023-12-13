#pragma once

#include "mgl_game/event.hpp"

namespace mgl::game::imgui
{
  void init();

  void shutdown();

  void on_event(mgl::game::event& event);

  bool is_initialized();

  void render();

} // namespace mgl::game::imgui