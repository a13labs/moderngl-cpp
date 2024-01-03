#pragma once
#include "buffers.hpp"
#include "material.hpp"
#include "shader.hpp"

#include "mgl_core/containers.hpp"
#include "mgl_core/memory.hpp"

#include "mgl_window/api.hpp"
#include "mgl_window/window.hpp"

#include "glm/glm.hpp"

namespace mgl::engine
{

  class renderer;
  class render_command;
  class batch_render;

  using renderer_ref = mgl::ref<renderer>;
  using render_command_ref = mgl::ref<render_command>;
  using batch_render_ref = mgl::ref<batch_render>;

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
      ENABLE_SHADER,
      DISABLE_SHADER,

      SET_RENDER_TARGET,
      COUNT
    };

    virtual void execute() = 0;

    type get_type() const { return m_type; }

protected:
    render_command(type type)
        : m_type(type)
    { }

private:
    type m_type;
  };

  class renderer
  {

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
      ONE,
      SRC_COLOR,
      ONE_MINUS_SRC_COLOR,
      DST_COLOR,
      ONE_MINUS_DST_COLOR,
      SRC_ALPHA,
      ONE_MINUS_SRC_ALPHA,
      DST_ALPHA,
      ONE_MINUS_DST_ALPHA,
      CONSTANT_COLOR,
      ONE_MINUS_CONSTANT_COLOR,
      CONSTANT_ALPHA,
      ONE_MINUS_CONSTANT_ALPHA,
      SRC_ALPHA_SATURATE,
      SRC1_COLOR,
      ONE_MINUS_SRC1_COLOR,
      SRC1_ALPHA,
      ONE_MINUS_SRC1_ALPHAB
    };

    enum class depth_func : uint32_t
    {
      NEVER,
      LESS,
      EQUAL,
      LEQUAL,
      GREATER,
      NOTEQUAL,
      GEQUAL,
      ALWAYS7
    };

    enum class stencil_func : uint32_t
    {
      NEVER,
      LESS,
      EQUAL,
      LEQUAL,
      GREATER,
      NOTEQUAL,
      GEQUAL,
      ALWAYS7
    };

    enum class stencil_op : uint32_t
    {
      KEEP,
      ZERO,
      REPLACE,
      INCR,
      INCR_WRAP,
      DECR,
      DECR_WRAP,
      INVERTA
    };

    enum class cull_face : uint32_t
    {
      FRONT,
      BACK,
      FRONT_AND_BACK
    };

    enum class draw_mode : uint32_t
    {
      POINTS = 0x0000,
      LINES = 0x0001,
      LINE_LOOP = 0x0002,
      LINE_STRIP = 0x0003,
      TRIANGLES = 0x0004,
      TRIANGLE_STRIP = 0x0005,
      TRIANGLE_FAN = 0x0006,
      LINES_ADJACENCY = 0x000A,
      LINE_STRIP_ADJACENCY = 0x000B,
      TRIANGLES_ADJACENCY = 0x000C,
      TRIANGLE_STRIP_ADJACENCY = 0x000D,
      PATCHES = 0x000E,
    };

    struct render_state
    {
      render_state()
          : current_shader(nullptr)
          , view_matrix(1.0f)
          , view_uniform(nullptr)
          , projection_matrix(1.0f)
          , projection_uniform(nullptr)
          , current_batch(nullptr)
      { }

      // The current shader, view and projection matrices are stored in the renderer, as they are
      // used by multiple commands
      shader_ref current_shader;

      glm::mat4 view_matrix;
      glm::mat4 projection_matrix;

      // Program uniforms
      mgl::window::uniform_ref view_uniform;
      mgl::window::uniform_ref projection_uniform;
      mgl::window::uniform_ref model_uniform;

      // The current batch is stored in the renderer, as it is used by multiple commands
      batch_render_ref current_batch;
    };

    renderer(const mgl::window::context_ref& context)
        : m_context(context)
        , m_render_queue()
    { }

    ~renderer() { m_render_queue.clear(); }

    void begin();

    void end();

    void enable_state(state state);

    void disable_state(state state);

    void enable_texture(uint32_t slot, const mgl::window::texture_ref& t);

    void disable_texture(uint32_t slot);

    void clear(const glm::vec4& color);

    void clear(float r, float g, float b, float a) { clear(glm::vec4(r, g, b, a)); }

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

    void draw(const vertex_buffer_ref& vertex_array, index_buffer_ref index_buffer, draw_mode mode);

    void enable_material(material_ref material);

    void disable_material();

    render_state& current_state() { return m_state_data; }

    const mgl::window::context_ref& context() const { return m_context; }

private:
    void submit(const render_command_ref& command) { m_render_queue.push_back(command); }
    void flush();

    mgl::window::context_ref m_context;
    mgl::list<render_command_ref> m_render_queue;

    render_state m_state_data;
  };

  class batch_render
  {
    struct data
    {
      data()
          : vertex_buffer(nullptr)
          , index_buffer(nullptr)
          , mode(renderer::draw_mode::TRIANGLES)
      { }

      data(const vertex_buffer_ref& vb, const index_buffer_ref& ib, renderer::draw_mode m)
          : vertex_buffer(vb)
          , index_buffer(ib)
          , mode(m)
      { }

      data(const data& other)
          : vertex_buffer(other.vertex_buffer)
          , index_buffer(other.index_buffer)
          , mode(other.mode)
      { }

      bool operator==(const data& other) const
      {
        return vertex_buffer == other.vertex_buffer && index_buffer == other.index_buffer &&
               mode == other.mode;
      }

      bool operator!=(const data& other) const { return !(*this == other); }

      vertex_buffer_ref vertex_buffer;
      index_buffer_ref index_buffer;
      renderer::draw_mode mode;
      glm::mat4 transform;
    };

public:
    batch_render()
        : m_batch_data()
    { }

    ~batch_render() { m_batch_data.clear(); }

    void push(const vertex_buffer_ref& vb, const index_buffer_ref& ib, renderer::draw_mode m);

    void clear() { m_batch_data.clear(); }

    void commit();

    bool empty() const { return m_batch_data.empty(); }

private:
    mgl::list<data> m_batch_data;
  };

} // namespace mgl::engine