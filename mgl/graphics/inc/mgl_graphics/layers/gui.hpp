#pragma once

#include "mgl_graphics/layer.hpp"
#include "mgl_graphics/shader.hpp"
#include "mgl_window/api.hpp"
#include "mgl_window/event.hpp"

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

    void on_event(mgl::window::event& event) override;

private:
    static void init_subsystem();
    static void shutdown_subsystem();
    static void render_subsystem();
    static bool is_initialized();
    static bool on_window_close(mgl::window::window_close_event& event);
    static bool on_window_resize(mgl::window::window_resize_event& event);
    static bool on_key_pressed(mgl::window::key_pressed_event& event);
    static bool on_key_released(mgl::window::key_released_event& event);
    static bool on_mouse_moved(mgl::window::mouse_moved_event& event);
    static bool on_mouse_scrolled(mgl::window::mouse_scrolled_event& event);
    static bool on_mouse_button_pressed(mgl::window::mouse_button_pressed_event& event);
    static bool on_mouse_button_released(mgl::window::mouse_button_released_event& event);

    struct imgui_render_data
    {
      mgl::window::api::program_ref prg;
      mgl::window::api::uniform_ref u_prj_matrix;
      mgl::window::api::uniform_ref u_position;
      mgl::window::api::uniform_ref u_uv;
      mgl::window::api::uniform_ref u_texture;
      mgl::window::api::uniform_ref u_color;
      mgl::window::api::buffer_ref vb;
      mgl::window::api::buffer_ref ib;
      mgl::window::api::texture_2d_ref font_texture;

      imgui_render_data()
          : prg(nullptr)
          , u_prj_matrix(nullptr)
          , u_position(nullptr)
          , u_uv(nullptr)
          , u_texture(nullptr)
          , u_color(nullptr)
          , vb(nullptr)
          , ib(nullptr)
          , font_texture(nullptr)
      { }
    };
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