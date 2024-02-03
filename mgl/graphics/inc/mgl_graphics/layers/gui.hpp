#pragma once

#include "mgl_graphics/layer.hpp"
#include "mgl_platform/event.hpp"

namespace mgl::graphics::layers
{
  class gui_layer : public layer
  {
public:
    gui_layer(const std::string& name = "ImGui Layer");

    virtual ~gui_layer() override;

    void on_attach() override;

    virtual void on_detach() override;

    virtual void on_update(float time, float frame_time) override final;

    virtual void draw_ui(float time, float frame_time) = 0;

    void on_event(mgl::platform::event& event) override;

private:
    static void init_subsystem();
    static void shutdown_subsystem();
    static void render_subsystem();
    static bool is_initialized();
    static void refresh_font();
    static bool on_window_close(mgl::platform::window_close_event& event);
    static bool on_window_resize(mgl::platform::window_resize_event& event);
    static bool on_key_pressed(mgl::platform::key_pressed_event& event);
    static bool on_key_released(mgl::platform::key_released_event& event);
    static bool on_mouse_moved(mgl::platform::mouse_moved_event& event);
    static bool on_mouse_scrolled(mgl::platform::mouse_scrolled_event& event);
    static bool on_mouse_button_pressed(mgl::platform::mouse_button_pressed_event& event);
    static bool on_mouse_button_released(mgl::platform::mouse_button_released_event& event);
  };

  class null_gui_layer : public gui_layer
  {
public:
    null_gui_layer(const std::string& name = "Null ImGui Layer")
        : gui_layer(name)
    { }

    virtual ~null_gui_layer() override = default;

    virtual void draw_ui(float time, float frame_time) override { }
  };
} // namespace mgl::graphics::layers