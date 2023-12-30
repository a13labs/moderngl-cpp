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
#include "layer.hpp"
#include "renderer.hpp"

#include "mgl_core/math.hpp"
#include "mgl_core/timer.hpp"

namespace mgl::window
{
  struct window_config
  {
    std::string title = "BaseWindow";
    uint32_t width = 800;
    uint32_t height = 600;
    bool fullscreen = false;
    bool resizable = true;
    bool v_sync = false;
    uint32_t samples = 0;
    bool cursor = true;
    key::name exit_key = key::Esc;
    key::name fullscreen_key = key::F11;

    window_config() = default;

    window_config(const std::string& title,
                  uint32_t width,
                  uint32_t height,
                  bool fullscreen,
                  bool resizable,
                  bool v_sync,
                  uint32_t samples,
                  bool cursor,
                  key::name exit_key,
                  key::name fullscreen_key)
        : title(title)
        , width(width)
        , height(height)
        , fullscreen(fullscreen)
        , resizable(resizable)
        , v_sync(v_sync)
        , samples(samples)
        , cursor(cursor)
        , exit_key(exit_key)
        , fullscreen_key(fullscreen_key)
    { }

    ~window_config() = default;

    window_config(const window_config& other)
    {
      title = other.title;
      width = other.width;
      height = other.height;
      fullscreen = other.fullscreen;
      resizable = other.resizable;
      v_sync = other.v_sync;
      samples = other.samples;
      cursor = other.cursor;
      exit_key = other.exit_key;
      fullscreen_key = other.fullscreen_key;
    }

    window_config& operator=(const window_config& other)
    {
      title = other.title;
      width = other.width;
      height = other.height;
      fullscreen = other.fullscreen;
      resizable = other.resizable;
      v_sync = other.v_sync;
      samples = other.samples;
      cursor = other.cursor;
      exit_key = other.exit_key;
      fullscreen_key = other.fullscreen_key;
      return *this;
    }
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

    int width();
    int height();
    float aspect_ratio();

    const std::string& title() const;
    void set_title(const std::string& value);

    void toggle_full_screen();
    const mgl::window::context& context() const;
    const mgl::window::renderer_ref& renderer() const;

    inline static window& current() { return *s_instance; }
    inline static bool is_available() { return s_instance != nullptr; }

    // Events
    virtual void on_event(event& event);

    // Windows Events
    virtual bool on_window_close(window_close_event& event);
    virtual bool on_window_resize(window_resize_event& event);

    // Keys Events
    virtual bool on_key_pressed(key_pressed_event& event) { return false; }
    virtual bool on_key_released(key_released_event& event) { return false; }

    // Mouse Events
    virtual bool on_mouse_moved(mouse_moved_event& event) { return false; }
    virtual bool on_mouse_scrolled(mouse_scrolled_event& event) { return false; }
    virtual bool on_mouse_button_pressed(mouse_button_pressed_event& event) { return false; }
    virtual bool on_mouse_button_released(mouse_button_released_event& event) { return false; }

    virtual void on_draw(float time, float frame_time) { }
    virtual void on_load(){};
    virtual void on_unload(){};

    layer_stack& layers() { return m_layers; }

    const window_config& config() const { return m_config; }

private:
    static window* s_instance;
    bool m_running;
    mgl::Timer m_timer;
    mgl::scope<native_window> m_native_window;
    mgl::window::context m_context;
    mgl::window::renderer_ref m_renderer;
    layer_stack m_layers;
    window_config m_config;
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

  inline const mgl::window::context& window::context() const
  {
    return m_context;
  }

  inline const mgl::window::renderer_ref& window::renderer() const
  {
    return m_renderer;
  }

  window_config load_window_configuration(const std::string& filename);

  inline const mgl::window::context& current_context()
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

} // namespace  mgl::window
