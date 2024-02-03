
/*
   Copyright 2022 Alexandre Pires (c.alexandre.pires@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include "mgl_platform/window.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_core/profiling.hpp"
#include "mgl_opengl/context.hpp"
#include "mgl_platform/api/opengl.hpp"
#include "mgl_platform/context/sdl_window.hpp"
#include "mgl_platform/event.hpp"
#include "mgl_platform/input.hpp"

namespace mgl::platform
{
  static mgl::scope<window> s_instance = nullptr;

  window::window(const window_config& config)
  {
    MGL_CORE_ASSERT(!s_instance, "Window already running!");
    s_instance = mgl::scope<window>(this);
    m_running = false;
    m_config = config;

    // Create api window (SDL)
    m_api_window = mgl::create_scope<sdl_window>(config);
  }

  window::~window()
  {
    s_instance.release();
  }

  void window::on_event(event& event)
  {
    EventDispatcher dispatcher(event);

    // Dispatch Windows Events
    dispatcher.dispatch<window_close_event>(MGL_CLS_BIND_EVENT_FN(window::on_window_close));
    dispatcher.dispatch<window_resize_event>(MGL_CLS_BIND_EVENT_FN(window::on_window_resize));

    // Dispatch key events to be handled by the application
    dispatcher.dispatch<key_pressed_event>(MGL_CLS_BIND_EVENT_FN(window::on_key_pressed));
    dispatcher.dispatch<key_released_event>(MGL_CLS_BIND_EVENT_FN(window::on_key_released));

    // Dispatch mouse events to be handled by the application
    dispatcher.dispatch<mouse_moved_event>(MGL_CLS_BIND_EVENT_FN(window::on_mouse_moved));
    dispatcher.dispatch<mouse_scrolled_event>(MGL_CLS_BIND_EVENT_FN(window::on_mouse_scrolled));
    dispatcher.dispatch<mouse_button_pressed_event>(
        MGL_CLS_BIND_EVENT_FN(window::on_mouse_button_pressed));
    dispatcher.dispatch<mouse_button_released_event>(
        MGL_CLS_BIND_EVENT_FN(window::on_mouse_button_released));
  }

  bool window::on_window_close(window_close_event& event)
  {
    m_running = false;
    return true;
  }

  void window::run()
  {
    if(m_running)
      return;

    if(!m_api_window->create_window())
    {
      MGL_CORE_TRACE("Window: Error creating Window");
      return;
    }

    m_context = mgl::opengl::create_context(mgl::opengl::context_mode::ATTACHED, 330);

    if(!m_context)
    {
      MGL_CORE_TRACE("Window: Error initializing GL shared context.");
      m_api_window->destroy_window();
      return;
    }

    m_api_window->initialize_event_handler(MGL_CLS_BIND_EVENT_FN(window::on_event));

    m_running = true;

    mgl::platform::api::init_api();

    MGL_PROFILE_BEGIN_SESSION();

    if(!on_load())
    {
      MGL_CORE_TRACE("Window: Error loading application.");
      m_api_window->destroy_window();
      return;
    }

    m_timer.start();

    while(m_running)
    {
      m_api_window->process_events();
      auto frame_time = m_timer.next_frame();
      on_update(frame_time.current, frame_time.delta);
      m_api_window->swap_buffers();

#if MGL_PROFILING
      // Since we are profiling we just render one frame
      m_running = false;
#endif
    }
    on_unload();

    MGL_PROFILE_END_SESSION();

    mgl::platform::api::shutdown_api();

    m_context->release();
    m_api_window->destroy_window();
  }

  bool window::on_window_resize(window_resize_event& event)
  {
    auto size = m_api_window->get_drawable_size();
    m_context->screen().set_viewport({ 0, 0, size.width, size.height });
    return true;
  }

  window_config load_window_configuration(const std::string& filename)
  {
    // TODO: Implement load from JSON
    return window_config();
  }

  const api::context_ref& window::current_context() const
  {
    return m_context;
  }

  window& window::current()
  {
    MGL_CORE_ASSERT(s_instance, "Window not running!");
    return *s_instance;
  }

  bool window::is_available()
  {
    return s_instance != nullptr;
  }

} // namespace  mgl::platform
