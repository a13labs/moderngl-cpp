#pragma once

#include "mgl_core/containers.hpp"
#include "mgl_core/memory.hpp"

#include "mgl_window/api.hpp"
#include "mgl_window/window.hpp"

#include "glm/glm.hpp"

namespace mgl::engine
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

      SET_RENDER_TARGET,
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

  class base_material
  {
    friend class renderer;

public:
    virtual ~base_material() = default;
    virtual void use() = 0;
  };

  using material = mgl::ref<base_material>;

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

    enum class blend_func : uint32_t
    {
      ZERO = 0,
      ONE = 1,
      SRC_COLOR = 0x0300,
      ONE_MINUS_SRC_COLOR = 0x0301,
      DST_COLOR = 0x0306,
      ONE_MINUS_DST_COLOR = 0x0307,
      SRC_ALPHA = 0x0302,
      ONE_MINUS_SRC_ALPHA = 0x0303,
      DST_ALPHA = 0x0304,
      ONE_MINUS_DST_ALPHA = 0x0305,
      CONSTANT_COLOR = 0x8001,
      ONE_MINUS_CONSTANT_COLOR = 0x8002,
      CONSTANT_ALPHA = 0x8003,
      ONE_MINUS_CONSTANT_ALPHA = 0x8004,
      SRC_ALPHA_SATURATE = 0x0308,
      SRC1_COLOR = 0x88F9,
      ONE_MINUS_SRC1_COLOR = 0x88FA,
      SRC1_ALPHA = 0x8589,
      ONE_MINUS_SRC1_ALPHA = 0x88FB
    };

    enum class depth_func : uint32_t
    {
      NEVER = 0x0200,
      LESS = 0x0201,
      EQUAL = 0x0202,
      LEQUAL = 0x0203,
      GREATER = 0x0204,
      NOTEQUAL = 0x0205,
      GEQUAL = 0x0206,
      ALWAYS = 0x0207
    };

    enum class stencil_func : uint32_t
    {
      NEVER = 0x0200,
      LESS = 0x0201,
      EQUAL = 0x0202,
      LEQUAL = 0x0203,
      GREATER = 0x0204,
      NOTEQUAL = 0x0205,
      GEQUAL = 0x0206,
      ALWAYS = 0x0207
    };

    enum class stencil_op : uint32_t
    {
      KEEP = 0x1E00,
      ZERO = 0,
      REPLACE = 0x1E01,
      INCR = 0x1E02,
      INCR_WRAP = 0x8507,
      DECR = 0x1E03,
      DECR_WRAP = 0x8508,
      INVERT = 0x150A
    };

    enum class cull_face : uint32_t
    {
      FRONT = 0x0404,
      BACK = 0x0405,
      FRONT_AND_BACK = 0x0408
    };

    renderer(mgl::window::context context)
        : m_context(context)
        , m_render_queue()
    { }

    ~renderer() { m_render_queue.clear(); }

    void set_clear_color(const glm::vec4& color) { m_clear_color = color; }

    void begin_scene();

    void end_scene();

    void enable_state(state state);

    void disable_state(state state);

    void enable_texture(uint32_t slot, mgl::window::texture t);

    void disable_texture(uint32_t slot);

    void clear(const glm::vec4& color);

    void set_viewport(const glm::vec2& position, const glm::vec2& size);

    void set_view(const glm::mat4& view);

    void set_projection(const glm::mat4& projection);

    void set_blend_func(blend_func src, blend_func dst);

    void set_color_mask(bool r, bool g, bool b, bool a);

    void set_depth_mask(bool mask);

    void set_depth_func(depth_func func);

    void set_stencil_mask(uint32_t mask);

    void set_stencil_func(stencil_func func, int32_t ref, uint32_t mask);

    void set_stencil_op(stencil_op sfail, stencil_op dpfail, stencil_op dppass);

    void set_cull_face(cull_face face);

    void set_polygon_offset(float factor, float units);

    void draw(const mgl::window::vertex_array& vertex_array, uint32_t count);

    void enable_material(material material);

    void disable_material();

    const mgl::window::context& context() const { return m_context; }

private:
    void submit(const render_command_ref& command) { m_render_queue.push_back(command); }
    void flush();

    mgl::window::context m_context;
    mgl::list<render_command_ref> m_render_queue;
    glm::vec3 m_clear_color;
  };

} // namespace mgl::engine