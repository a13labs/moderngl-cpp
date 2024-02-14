#pragma once
#include "mgl_platform/event.hpp"
#include "mgl_platform/input.hpp"
#include "mgl_platform/window.hpp"

#include "SDL.h"

namespace mgl::platform
{
  struct window_state
  {
    event_handler handler;
    window_config current_config = window_config();
    bool fullscreen = false;
    int width;
    int height;
  };

  class sdl_window : public api_window
  {

public:
    sdl_window(const window_config& config = window_config());
    virtual ~sdl_window() = default;

public:
    virtual bool create_window() override;
    virtual void destroy_window() override;
    virtual void swap_buffers() override;
    virtual void process_events() override;

    virtual int width() override;
    virtual int height() override;
    virtual float aspect_ratio() override;

    virtual void initialize_event_handler(const event_handler& handler) override;
    virtual void toggle_full_screen() override;
    virtual mgl::size get_drawable_size() override;

    virtual bool is_minimized() override;

    virtual void set_title(const std::string& value) override;
    virtual const std::string& title() const override;

private:
    SDL_GLContext m_context;
    std::string m_title;
    SDL_Window* m_api_window = nullptr;
    window_state m_state;
  };
} // namespace  mgl::platform
