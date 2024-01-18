#pragma once
#include "buffer.hpp"
#include "buffers/index.hpp"
#include "buffers/vertex.hpp"
#include "glm/glm.hpp"
#include "managers/buffer.hpp"
#include "managers/shader.hpp"
#include "managers/texture.hpp"
#include "material.hpp"
#include "mgl_core/containers.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_window/api.hpp"
#include "mgl_window/window.hpp"
#include "shader.hpp"
#include "texture.hpp"

namespace mgl::graphics
{

  class render;
  class render_command;
  class render_script;
  class batch_render;

  using render_ref = mgl::scope<render>;
  using render_command_ref = mgl::ref<render_command>;
  using batch_render_ref = mgl::ref<batch_render>;
  using render_script_ref = mgl::ref<render_script>;

  class render_command
  {
public:
    virtual void execute() = 0;
  };

  class render_script : public render_command

  {
public:
    render_script()
        : m_render_target(nullptr)
        , m_commands()
    { }

    render_script(const mgl::window::api::framebuffer_ref& target)
        : m_render_target(target)
        , m_commands()
    { }
    ~render_script() { m_commands.clear(); }

    void push(const render_command_ref& command) { m_commands.push_back(command); }

    void clear() { m_commands.clear(); }

    void execute();

private:
    mgl::list<render_command_ref> m_commands;
    mgl::window::api::framebuffer_ref m_render_target;
  };

  class render
  {

public:
    enum class state
    {
      BLEND = BIT(1),
      DEPTH_TEST = BIT(2),
      CULL_FACE = BIT(3),
      STENCIL_TEST = BIT(4),
      RASTERIZER_DISCARD = BIT(5),
      PROGRAM_POINT_SIZE = BIT(6),
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

    enum blend_equation_mode
    {
      ADD,
      SUBTRACT,
      REVERSE_SUBTRACT,
      MIN,
      MAX,
    };

    enum blend_factor
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
      ONE_MINUS_SRC1_ALPHA,
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

      // The current shader, view and projection matrices are stored in the render, as they are
      // used by multiple commands
      shader_ref current_shader;

      glm::mat4 view_matrix;
      glm::mat4 projection_matrix;

      // Program uniforms
      mgl::window::api::uniform_ref view_uniform;
      mgl::window::api::uniform_ref projection_uniform;
      mgl::window::api::uniform_ref model_uniform;

      // The current batch is stored in the render, as it is used by multiple commands
      batch_render_ref current_batch;

      // Used textures
      mgl::list<mgl::window::api::texture_ref> textures;
    };

    render(const mgl::window::api::context_ref& context);

    ~render();

    void begin();

    void end();

    void enable_state(int state);

    void enable_state(state state) { enable_state(static_cast<int>(state)); }

    void disable_state(int state);

    void enable_texture(uint32_t slot, const texture_ref& tex);

    void enable_texture(uint32_t slot, const std::string& name)
    {
      enable_texture(slot, m_texture_manager.get_item(name));
    }

    void enable_texture(uint32_t slot, uint32_t idx)
    {
      enable_texture(slot, m_texture_manager.get_item(idx));
    }

    void clear(const glm::vec4& color);

    void set_viewport(const glm::vec2& position, const glm::vec2& size);

    void set_view(const glm::mat4& view);

    void set_projection(const glm::mat4& projection);

    void clear_samplers(int start = 0, int end = -1);

    void set_blend_equation(blend_equation_mode modeRGB, blend_equation_mode modeAlpha);

    void set_blend_equation(blend_equation_mode mode) { set_blend_equation(mode, mode); }

    void set_blend_func(blend_factor srcRGB,
                        blend_factor dstRGB,
                        blend_factor srcAlpha,
                        blend_factor dstAlpha);

    void set_blend_func(blend_factor src, blend_factor dst) { set_blend_func(src, dst, src, dst); }

    void draw(const vertex_buffer_ref& vertex_array,
              const index_buffer_ref& index_buffer,
              draw_mode mode);

    void enable_shader(shader_ref shader);

    void enable_shader(const std::string& name) { enable_shader(m_shader_manager.get_item(name)); }

    void enable_shader(uint32_t idx) { enable_shader(m_shader_manager.get_item(idx)); }

    void disable_shader();

    void enable_material(material_ref shader);

    void disable_material();

    void push_render_script(const render_script_ref& script);

    render_state& current_state() { return m_state_data; }

    const mgl::window::api::context_ref& context() const { return m_context; }

    void clear(float r, float g, float b, float a) { clear(glm::vec4(r, g, b, a)); }

    static render& current_render();

    size_t register_shader(const std::string& name, const shader_ref& shader)
    {
      return m_shader_manager.add_item(name, shader);
    }

    void unregister_shader(const std::string& name) { m_shader_manager.remove_item(name); }

    bool has_shader(const std::string& name) { return m_shader_manager.has_item(name); }

    shader_ref get_shader(const std::string& name) { return m_shader_manager.get_item(name); }

    shader_ref get_shader(size_t idx) { return m_shader_manager.get_item(idx); }

    size_t register_texture(const std::string& name, const texture_ref& texture)
    {
      return m_texture_manager.add_item(name, texture);
    }

    void unregister_texture(const std::string& name) { m_texture_manager.remove_item(name); }

    bool has_texture(const std::string& name) { return m_texture_manager.has_item(name); }

    texture_ref get_texture(const std::string& name) { return m_texture_manager.get_item(name); }

    texture_ref get_texture(size_t idx) { return m_texture_manager.get_item(idx); }

    size_t register_buffer(const std::string& name, const buffer_ref& buffer)
    {
      return m_buffer_manager.add_item(name, buffer);
    }

    void unregister_buffer(const std::string& name) { m_buffer_manager.remove_item(name); }

    bool has_buffer(const std::string& name) { return m_buffer_manager.has_item(name); }

    buffer_ref get_buffer(const std::string& name) { return m_buffer_manager.get_item(name); }

    buffer_ref get_buffer(size_t idx) { return m_buffer_manager.get_item(idx); }

    void release();

private:
    void submit(const render_command_ref& command) { m_render_queue.push(command); }
    void flush();

    mgl::window::api::context_ref m_context;
    render_script m_render_queue;
    render_state m_state_data;
    shader_manager m_shader_manager;
    texture_manager m_texture_manager;
    buffer_manager m_buffer_manager;
  };

  class batch_render
  {
    struct data
    {
      data()
          : vertex_buffer(nullptr)
          , index_buffer(nullptr)
          , mode(render::draw_mode::TRIANGLES)
      { }

      data(const vertex_buffer_ref& vb, const index_buffer_ref& ib, render::draw_mode m)
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
      render::draw_mode mode;
      glm::mat4 transform;
    };

public:
    batch_render()
        : m_batch_data()
    { }

    ~batch_render() { m_batch_data.clear(); }

    void push(const vertex_buffer_ref& vb, const index_buffer_ref& ib, render::draw_mode m);

    void clear() { m_batch_data.clear(); }

    void commit();

    bool empty() const { return m_batch_data.empty(); }

private:
    mgl::list<data> m_batch_data;
  };

  inline render& current_render()
  {
    return render::current_render();
  }

} // namespace mgl::graphics