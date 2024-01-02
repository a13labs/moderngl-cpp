#include "mgl_engine/application.hpp"
#include "mgl_engine/layers/gui.hpp"
#include "mgl_engine/layers/render.hpp"

#include "mgl_core/containers.hpp"
#include "mgl_core/memory.hpp"

#include "mgl_core/debug.hpp"

namespace mgl::engine
{
  static application* s_instance = nullptr;

  application::application(const application_config& settings)
      : mgl::window::window(settings)
      , m_render_layer(settings.render_layer)
      , m_gui_layer(settings.gui_layer)
      , m_layers()
      , m_renderer(nullptr)
  {
    MGL_CORE_ASSERT(s_instance == nullptr, "Application already exists!");
    s_instance = this;
  }

  application::~application() { }

  void application::on_event(mgl::window::event& event)
  {
    mgl::window::window::on_event(event);
    m_gui_layer->on_event(event);
    m_layers.on_event(event);
    m_render_layer->on_event(event);
  }

  const renderer_ref& application::current_renderer()
  {
    return m_renderer;
  }

  void application::on_update(float time, float frame_time)
  {
    m_render_layer->on_update(time, frame_time);
    m_layers.on_update(time, frame_time);
    m_gui_layer->on_update(time, frame_time);
  }

  bool application::on_load()
  {
    // Create the renderer
    m_renderer = mgl::create_ref<mgl::engine::renderer>(this->current_context());

    if(config().gui_layer == nullptr)
    {
      m_gui_layer = mgl::create_ref<mgl::engine::layers::null_gui_layer>();
    }
    else
    {
      m_gui_layer = config().gui_layer;
      m_gui_layer->on_attach();
    }

    if(config().render_layer == nullptr)
    {
      m_render_layer = mgl::create_ref<mgl::engine::layers::null_render_layer>();
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
  }

  application& application::current()
  {
    MGL_CORE_ASSERT(s_instance, "Application not running!");
    return *s_instance;
  }

} // namespace mgl::engine