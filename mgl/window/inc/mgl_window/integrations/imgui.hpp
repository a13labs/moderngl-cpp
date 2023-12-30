#pragma once

#include "mgl_window/event.hpp"

namespace mgl::window::imgui
{
  void init();

  void shutdown();

  void on_event(mgl::window::event& event);

  bool is_initialized();

  void render();

} // namespace mgl::window::imgui