#pragma once

#include "mgl_graphics/graphics.hpp"
#include "mgl_graphics/layer.hpp"

#include "mgl_core/containers.hpp"
#include "mgl_window/window.hpp"

namespace mgl::application
{

  struct application_config : public mgl::window::window_config
  {
    mgl::graphics::layer_ref gui_layer = nullptr;
    mgl::graphics::layer_ref render_layer = nullptr;
    mgl::ref_list<mgl::graphics::layer> extra_layers;

    application_config()
        : mgl::window::window_config()
        , gui_layer(nullptr)
        , render_layer(nullptr)
        , extra_layers()
    { }

    application_config(const mgl::window::window_config& settings)
        : mgl::window::window_config(settings)
        , gui_layer(nullptr)
        , render_layer(nullptr)
        , extra_layers()
    { }

    application_config(const application_config& settings)
        : mgl::window::window_config(settings)
        , gui_layer(settings.gui_layer)
        , render_layer(settings.render_layer)
        , extra_layers(settings.extra_layers)
    { }

    application_config& operator=(const mgl::window::window_config& settings)
    {
      mgl::window::window_config::operator=(settings);
      return *this;
    }

    application_config& operator=(const application_config& settings)
    {
      mgl::window::window_config::operator=(settings);
      gui_layer = settings.gui_layer;
      render_layer = settings.render_layer;
      extra_layers = settings.extra_layers;
      return *this;
    }
  };

  class application : public mgl::window::window
  {
public:
    application(const application_config& settings = application_config());
    ~application();

    mgl::graphics::layer_stack& layers() { return m_layers; }

    static application& current();

    application_config& config() { return m_config; }

protected:
    virtual void on_event(mgl::window::event& event) override final;
    virtual void on_update(float time, float frame_time) override final;
    virtual bool on_load() override final;
    virtual void on_unload() override final;

private:
    mgl::graphics::layer_ref m_gui_layer;
    mgl::graphics::layer_ref m_render_layer;
    application_config m_config;
    mgl::graphics::layer_stack m_layers;
  };

  inline application& current_application()
  {
    return application::current();
  }

} // namespace mgl::application
