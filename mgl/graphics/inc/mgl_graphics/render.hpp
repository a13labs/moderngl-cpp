#pragma once
#include "batch_render.hpp"
#include "buffer.hpp"
#include "buffers/index.hpp"
#include "buffers/vertex.hpp"
#include "enums.hpp"
#include "glm/glm.hpp"
#include "managers/buffer.hpp"
#include "managers/shader.hpp"
#include "managers/texture.hpp"
#include "material.hpp"
#include "render_script.hpp"
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

    static render& current_render();

    render_state& current_state() { return m_state_data; }

    const mgl::window::api::context_ref& context() const { return m_context; }

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