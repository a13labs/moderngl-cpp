#pragma once
#include "batch.hpp"
#include "buffer.hpp"
#include "buffers/index.hpp"
#include "buffers/vertex.hpp"
#include "command.hpp"
#include "enums.hpp"
#include "glm/glm.hpp"
#include "managers/buffer.hpp"
#include "managers/shader.hpp"
#include "managers/texture.hpp"
#include "shader.hpp"
#include "texture.hpp"

namespace mgl::graphics
{
  class render;
  using render_ref = mgl::scope<render>;
  class render
  {
public:
    struct render_state
    {
      render_state()
          : current_shader(nullptr)
          , view_matrix(1.0f)
          , view_uniform(nullptr)
          , projection_matrix(1.0f)
          , projection_uniform(nullptr)
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

      // Used textures
      mgl::list<mgl::window::api::texture_ref> textures;
    };

    render(const mgl::window::api::context_ref& context);

    ~render();

    static render& current_render();

    render_state& current_state() { return m_state_data; }

    const mgl::window::api::context_ref& context() const { return m_context; }

    void release();

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

    void enable_scissor();

    void disable_scissor();

    void set_scissor(const glm::vec2& position, const glm::vec2& size);

    void set_scissor(const glm::vec4& rect) { set_scissor(glm::vec2(rect), glm::vec2(rect.z, rect.w)); }

    void set_scissor(float x, float y, float width, float height) { set_scissor(glm::vec4(x, y, width, height)); }
    
    void enable_state(int state);

    void disable_state(int state);

    void clear(const glm::vec4& color);

    void clear(float r, float g, float b, float a) { clear(glm::vec4(r, g, b, a)); }

    void set_viewport(const glm::vec2& position, const glm::vec2& size);

    void clear_samplers(int start = 0, int end = -1);

    void set_blend_equation(blend_equation_mode modeRGB, blend_equation_mode modeAlpha);

    void set_blend_equation(blend_equation_mode mode) { set_blend_equation(mode, mode); }

    void set_blend_func(blend_factor srcRGB,
                        blend_factor dstRGB,
                        blend_factor srcAlpha,
                        blend_factor dstAlpha);

    void set_blend_func(blend_factor src, blend_factor dst) { set_blend_func(src, dst, src, dst); }

    void draw(const vertex_buffer_ref& vb,
              const index_buffer_ref& ib = nullptr,
              render_mode mode = render_mode::TRIANGLES,
              const glm::mat4& model_view = glm::mat4(1.0f),
              size_t count = 0,
              size_t offset = 0,
              size_t instance_count = 1);

    void draw_batch(const batch_ref& batch);

private:
    mgl::window::api::context_ref m_context;
    render_state m_state_data;
    shader_manager m_shader_manager;
    texture_manager m_texture_manager;
    buffer_manager m_buffer_manager;
  };

  inline render& current_render()
  {
    return render::current_render();
  }

} // namespace mgl::graphics