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
#include "input.hpp"
#include "mgl_window/builtins.hpp"
#include "mgl_window/render/buffer.hpp"

namespace mgl::window
{
  struct window_config
  {
    mgl::core::string title = "BaseWindow";
    uint32_t width = 800;
    uint32_t height = 600;
    bool fullscreen = false;
    bool resizable = true;
    bool VSync = true;
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
    virtual mgl::core::size get_drawable_size() = 0;

    virtual int width() = 0;
    virtual int height() = 0;
    virtual float aspect_ratio() = 0;

    virtual const mgl::core::string& title() const = 0;
    virtual void set_title(const mgl::core::string& value) = 0;
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

    const mgl::core::string& title() const;
    void set_title(const mgl::core::string& value);

    void toggle_full_screen();
    mgl::core::ref<mgl::opengl::context> context();

    inline static window& current() { return *s_instance; }

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
    mgl::core::Timer m_timer;
    mgl::core::scope<native_window> m_native_window;
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

  inline const mgl::core::string& window::title() const
  {
    return m_native_window->title();
  }

  inline void window::set_title(const mgl::core::string& value)
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

  inline mgl::core::ref<mgl::opengl::context> window::context()
  {
    return m_context;
  }

  window_config load_window_configuration(const mgl::core::string& filename);
} // namespace  mgl::window
