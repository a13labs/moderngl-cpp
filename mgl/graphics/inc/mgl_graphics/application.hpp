#pragma once

#include "layer.hpp"
#include "render.hpp"

#include "mgl_core/containers.hpp"
#include "mgl_window/window.hpp"

namespace mgl::graphics
{

  struct application_config : public mgl::window::window_config
  {
    layer_ref gui_layer = nullptr;
    layer_ref render_layer = nullptr;
    mgl::ref_list<layer> extra_layers;

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

    const render_ref& current_render();

    layer_stack& layers() { return m_layers; }

    static application& current();

    application_config& config() { return m_config; }

protected:
    virtual void on_event(mgl::window::event& event) override final;
    virtual void on_update(float time, float frame_time) override final;
    virtual bool on_load() override final;
    virtual void on_unload() override final;

private:
    render_ref m_render;
    layer_ref m_gui_layer;
    layer_ref m_render_layer;
    application_config m_config;
    layer_stack m_layers;
  };

  inline application& current_application()
  {
    return application::current();
  }

  inline const render_ref& current_render()
  {
    return current_application().current_render();
  }

} // namespace mgl::graphics
