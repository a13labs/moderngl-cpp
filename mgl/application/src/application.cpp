#include "mgl_application/application.hpp"
#include "mgl_graphics/layers/gui.hpp"
#include "mgl_graphics/layers/render.hpp"

#include "mgl_core/containers.hpp"
#include "mgl_core/memory.hpp"

#include "mgl_core/debug.hpp"

namespace mgl::application
{
  static mgl::scope<application> s_instance = nullptr;

  application::application(const application_config& settings)
      : mgl::window::window(settings)
      , m_render_layer(settings.render_layer)
      , m_gui_layer(settings.gui_layer)
      , m_layers()
  {
    MGL_CORE_ASSERT(s_instance == nullptr, "Application already exists!");
    s_instance = mgl::scope<application>(this);
  }

  application::~application()
  {
    s_instance.release();
  }

  void application::on_event(mgl::window::event& event)
  {
    mgl::window::window::on_event(event);
    m_gui_layer->on_event(event);
    m_layers.on_event(event);
    m_render_layer->on_event(event);
  }

  void application::on_update(float time, float frame_time)
  {
    m_render_layer->on_update(time, frame_time);
    m_layers.on_update(time, frame_time);
    m_gui_layer->on_update(time, frame_time);
  }

  bool application::on_load()
  {
    mgl::graphics::init();
    if(config().gui_layer == nullptr)
    {
      m_gui_layer = mgl::create_ref<mgl::graphics::layers::null_gui_layer>();
    }
    else
    {
      m_gui_layer = config().gui_layer;
      m_gui_layer->on_attach();
    }

    if(config().render_layer == nullptr)
    {
      m_render_layer = mgl::create_ref<mgl::graphics::layers::null_render_layer>();
    }
    else
    {
      m_render_layer = config().render_layer;
      m_render_layer->on_attach();
    }

    // Add the startup layers
    for(auto& layer : m_config.extra_layers)
    {
      m_layers.push_back(layer);
    }

    return true;
  }

  void application::on_unload()
  {
    m_gui_layer->on_detach();
    m_render_layer->on_detach();
    m_layers.clear();
    mgl::graphics::shutdown();
  }

  application& application::current()
  {
    MGL_CORE_ASSERT(s_instance, "Application not running!");
    return *s_instance;
  }

} // namespace mgl::application