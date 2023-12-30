#pragma once

#include "api.hpp"

#include "mgl_core/containers.hpp"
#include "mgl_core/memory.hpp"

#include "glm/glm.hpp"

namespace mgl::window
{

  class renderer;
  class render_command;

  using renderer_ref = mgl::ref<renderer>;
  using render_command_ref = mgl::ref<render_command>;

  class render_command
  {
    friend class renderer;

public:
    enum class type
    {
      NONE,
      ENABLE_STATE,
      DISABLE_STATE,
      ENABLE_TEXTURE,
      DISABLE_TEXTURE,
      SET_RENDER_TARGET,
      CLEAR,
      SET_VIEWPORT,
      SET_VIEW,
      SET_PROJECTION,
      SET_BLEND_FUNC,
      SET_COLOR_MASK,
      SET_DEPTH_MASK,
      SET_DEPTH_FUNC,
      SET_STENCIL_MASK,
      SET_STENCIL_FUNC,
      SET_STENCIL_OP,
      SET_CULL_FACE,
      SET_POLYGON_OFFSET,
      DRAW,
      ENABLE_MATERIAL,
      DISABLE_MATERIAL,

      COUNT
    };

    virtual void execute() = 0;

    type get_type() const { return m_type; }

protected:
    render_command(type type)
        : m_type(type)
        , m_renderer(nullptr)
    { }

    void set_renderer(renderer_ref renderer) { m_renderer = renderer; }

    const renderer_ref& renderer() const { return m_renderer; }

private:
    type m_type;
    renderer_ref m_renderer;
  };

  class renderer
  {
    friend class window;

public:
    enum class state
    {
      DEPTH_TEST,
      SCISSOR_TEST,
      STENCIL_TEST,
      ALPHA_TEST,
      BLEND,
      CULL_FACE,
      POLYGON_OFFSET_FILL,
      COUNT
    };

    renderer(mgl::window::context context)
        : m_context(context)
        , m_render_queue()
    { }

    ~renderer() { m_render_queue.clear(); }

    void set_clear_color(const glm::vec4& color) { m_clear_color = color; }

    void begin_scene() { m_render_queue.clear(); }

    void end_scene() { flush(); }

    void submit(const render_command_ref& command) { m_render_queue.push_back(command); }

    const mgl::window::context& context() const { return m_context; }

private:
    void flush();

    mgl::window::context m_context;
    mgl::list<render_command_ref> m_render_queue;
    glm::vec3 m_clear_color;
  };

} // namespace mgl::window