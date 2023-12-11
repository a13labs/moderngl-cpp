#pragma once
#include "event.hpp"

#include "mgl_core/containers.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"

namespace mgl::game
{
  class layer
  {
public:
    layer(const std::string& name, bool enabled = true)
        : m_name(name)
        , m_enabled(enabled){};
    virtual ~layer() = default;

    virtual void on_attach() = 0;
    virtual void on_detach() = 0;

    virtual void on_draw(float time, float frame_time) = 0;
    virtual bool on_event(event& event) = 0;

    void enable() { m_enabled = true; }
    void disable() { m_enabled = false; }
    bool is_enabled() const { return m_enabled; }

    const std::string& name() const { return m_name; }

private:
    std::string m_name;
    bool m_enabled;
  };

  using layer_ref = mgl::ref<layer>;

  class layer_stack
  {
public:
    layer_stack() = default;
    ~layer_stack();

    void push_back(layer_ref layer);
    void push_front(layer_ref layer);

    void pop_front();
    void pop_back();

    void remove_layer(layer_ref layer);

    void on_event(event& event);
    void on_draw(float time, float frame_time);

    mgl::ref_list<layer>::iterator begin() { return m_layers.begin(); }
    mgl::ref_list<layer>::iterator end() { return m_layers.end(); }
    mgl::ref_list<layer>::reverse_iterator rbegin() { return m_layers.rbegin(); }
    mgl::ref_list<layer>::reverse_iterator rend() { return m_layers.rend(); }

    mgl::ref_list<layer>::const_iterator begin() const { return m_layers.begin(); }
    mgl::ref_list<layer>::const_iterator end() const { return m_layers.end(); }
    mgl::ref_list<layer>::const_reverse_iterator rbegin() const { return m_layers.rbegin(); }
    mgl::ref_list<layer>::const_reverse_iterator rend() const { return m_layers.rend(); }

    void clear();

    size_t size() const { return m_layers.size(); }

private:
    mgl::ref_list<layer> m_layers;
  };
} // namespace mgl::game
