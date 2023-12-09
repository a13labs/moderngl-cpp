#pragma once

#include "mgl_core/containers.hpp"
#include "mgl_core/string.hpp"

#include "mgl_opengl/context.hpp"

#include <functional>
namespace mgl::window
{
  class window;
  class event;
  class window_close_event;
  class window_resize_event;
  class key_pressed_event;
  class key_released_event;
  class key_typed_event;
  class mouse_moved_event;
  class mouse_scrolled_event;
  class mouse_button_pressed_event;
  class mouse_button_released_event;

  namespace render
  {
    struct buffer_element;
    using buffer_layout = mgl::list<buffer_element>;

    class vertex_buffer;
    using vertex_buffers_list = mgl::list<vertex_buffer>;
  } // namespace render

  using event_handler = std::function<void(event&)>;

  using shader_defines = mgl::dict<std::string, std::string>;

  mgl::opengl::context_ref current_context();
  window& current_window();

} // namespace  mgl::window