#pragma once
#include "api/render_api.hpp"
#include "event.hpp"
#include "input.hpp"

#include "mgl_core/math.hpp"
#include "mgl_core/timer.hpp"

namespace mgl::platform
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

  class api_window
  {
public:
    api_window(const window_config& config = window_config()){};
    virtual ~api_window() = default;

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
    virtual ~window();

public:
    void run();

    int width() { return m_api_window->width(); }
    int height() { return m_api_window->height(); }
    float aspect_ratio() { return m_api_window->aspect_ratio(); }

    const std::string& title() const { return m_api_window->title(); }
    void set_title(const std::string& value) { m_api_window->set_title(value); }

    void toggle_full_screen() { m_api_window->toggle_full_screen(); }

    const window_config& config() const { return m_config; }

    static window& current();

    static bool is_available();

protected:
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

    virtual void on_update(float time, float frame_time) { }
    virtual bool on_load() { return false; }
    virtual void on_unload() { }

private:
    bool m_running;
    mgl::Timer m_timer;
    mgl::scope<api_window> m_api_window;
    window_config m_config;
  };

  window_config load_window_configuration(const std::string& filename);

  inline window& current_window()
  {
    return window::current();
  }

  inline bool is_window_available()
  {
    return window::is_available();
  }

} // namespace  mgl::platform
