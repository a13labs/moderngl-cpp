#include "mgl_engine/layer.hpp"
#include "mgl_window/integrations/imgui.hpp"

namespace mgl::engine
{
  layer_stack::~layer_stack()
  {
    clear();
  }

  void layer_stack::push_back(layer_ref layer)
  {
    m_layers.push_back(layer);
    layer->on_attach();
  }

  void layer_stack::push_front(layer_ref layer)
  {
    if(m_layers.empty())
    {
      m_layers.push_back(layer);
    }
    else
    {
      m_layers.insert(m_layers.begin() + 1, layer);
    }

    layer->on_attach();
  }

  void layer_stack::pop_front()
  {
    if(!m_layers.empty())
    {
      m_layers.front()->on_detach();
      m_layers.erase(m_layers.begin());
    }
  }

  void layer_stack::pop_back()
  {
    if(!m_layers.empty())
    {
      m_layers.back()->on_detach();
      m_layers.pop_back();
    }
  }

  void layer_stack::remove_layer(layer_ref layer)
  {
    auto it = std::find(m_layers.begin(), m_layers.end(), layer);
    if(it != m_layers.end())
    {
      layer->on_detach();
      m_layers.erase(it);
    }
  }

  void layer_stack::on_event(mgl::window::event& event)
  {
    for(auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
    {
      if(!(*it)->is_enabled())
        continue;

      (*it)->on_event(event);

      if(event.is_handled())
        break;
    }
  }

  void layer_stack::on_update(float time, float frame_time)
  {
    for(auto& layer : m_layers)
    {
      if(!layer->is_enabled())
        continue;
      layer->on_update(time, frame_time);
    }
  }

  void layer_stack::clear()
  {
    for(auto& layer : m_layers)
    {
      layer->on_detach();
    }
    m_layers.clear();

    if(mgl::window::imgui::is_initialized())
      mgl::window::imgui::shutdown();
  }
} // namespace mgl::engine