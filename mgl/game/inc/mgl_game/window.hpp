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
#pragma once
#include "event.hpp"
#include "input.hpp"

#include "mgl_core/math.hpp"
#include "mgl_core/timer.hpp"
#include "mgl_opengl/context.hpp"

namespace mgl::game
{
  struct window_config
  {
    std::string title = "BaseWindow";
    uint32_t width = 800;
    uint32_t height = 600;
    bool fullscreen = false;
    bool resizable = true;
    bool v_sync = true;
    uint32_t samples = 0;
    bool cursor = true;
    key::name exit_key = key::Esc;
    key::name fullscreen_key = key::F11;
  };

  class native_window
  {
public:
    native_window(const window_config& config = window_config()){};
    virtual ~native_window() = default;

    virtual bool create_window() = 0;
    virtual void destroy_window() = 0;
    virtual void process_events() = 0;
    virtual void swap_buffers() = 0;
    virtual void initialize_event_handler(const event_handler& handler) = 0;
    virtual void toggle_full_screen() = 0;
    virtual mgl::size get_drawable_size() = 0;

    virtual int width() = 0;
    virtual int height() = 0;
    virtual float aspect_ratio() = 0;

    virtual const std::string& title() const = 0;
    virtual void set_title(const std::string& value) = 0;
  };

  class window
  {

public:
    window(const window_config& config = window_config());
    virtual ~window() = default;

public:
    void run();
    void quit();

    int width();
    int height();
    float aspect_ratio();

    const std::string& title() const;
    void set_title(const std::string& value);

    void toggle_full_screen();
    mgl::ref<mgl::opengl::context> context();

    inline static window& current() { return *s_instance; }
    inline static bool is_available() { return s_instance != nullptr; }

    // Events
    void on_event(event& event);

    // Windows Events
    virtual bool on_window_close(window_close_event& event);
    virtual bool on_window_resize(window_resize_event& event);

    // Keys Events
    virtual bool on_key_pressed(key_pressed_event& event) { return true; }
    virtual bool on_key_released(key_released_event& event) { return true; }

    // Mouse Events
    virtual bool on_mouse_moved(mouse_moved_event& event) { return true; }
    virtual bool on_mouse_scrolled(mouse_scrolled_event& event) { return true; }
    virtual bool on_mouse_button_pressed(mouse_button_pressed_event& event) { return true; }
    virtual bool on_mouse_button_released(mouse_button_released_event& event) { return true; }

    virtual void on_draw(float time, float frame_time){};
    virtual void on_load(){};
    virtual void on_unload(){};

private:
    static window* s_instance;
    bool m_running;
    mgl::Timer m_timer;
    mgl::scope<native_window> m_native_window;
    mgl::opengl::context_ref m_context;
  };

  inline int window::width()
  {
    return m_native_window->width();
  }

  inline int window::height()
  {
    return m_native_window->height();
  }

  inline float window::aspect_ratio()
  {
    return m_native_window->aspect_ratio();
  }

  inline const std::string& window::title() const
  {
    return m_native_window->title();
  }

  inline void window::set_title(const std::string& value)
  {
    return m_native_window->set_title(value);
  }

  inline void window::toggle_full_screen()
  {
    return m_native_window->toggle_full_screen();
  }

  inline void window::quit()
  {
    m_running = false;
  }

  inline mgl::ref<mgl::opengl::context> window::context()
  {
    return m_context;
  }

  window_config load_window_configuration(const std::string& filename);

  inline mgl::opengl::context_ref current_context()
  {
    return window::current().context();
  }

  inline window& current_window()
  {
    return window::current();
  }

  inline bool is_window_available()
  {
    return window::is_available();
  }

} // namespace  mgl::game