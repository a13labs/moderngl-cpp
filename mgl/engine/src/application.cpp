#include "mgl_engine/application.hpp"

#include "mgl_core/containers.hpp"
#include "mgl_core/memory.hpp"

namespace mgl::engine
{

  application::application(const std::initializer_list<layer_ref>& layers,
                           const mgl::window::window_config& settings)
      : mgl::window::window(settings)
      , m_layers()
      , m_startup_layers(layers)
      , m_renderer(nullptr)
  { }

  application::~application() { }

  void application::on_event(mgl::window::event& event)
  {
    mgl::window::window::on_event(event);
    m_layers.on_event(event);
  }

  const renderer_ref& application::current_renderer() const
  {
    return m_renderer;
  }

  void application::on_draw(float time, float frame_time)
  {
    m_layers.on_draw(time, frame_time);
  }

  bool application::on_load()
  {
    m_renderer = mgl::create_ref<mgl::engine::renderer>(this->current_context());

    for(auto& layer : m_startup_layers)
    {
      m_layers.push_back(layer);
    }

    return true;
  }

  void application::on_unload()
  {
    m_layers.clear();
  }
} // namespace mgl::engine