#pragma once
#include "batch.hpp"
#include "buffer.hpp"
#include "buffers/index.hpp"
#include "buffers/vertex.hpp"
#include "command.hpp"
#include "enums.hpp"
#include "glm/glm.hpp"
#include "managers/buffer.hpp"
#include "managers/font.hpp"
#include "managers/shader.hpp"
#include "managers/texture.hpp"
#include "mgl_window/api/opengl.hpp"
#include "shader.hpp"
#include "texture.hpp"

namespace mgl::graphics
{
  class render_registry;
  using render_ref = mgl::scope<render_registry>;
  class render_registry
  {
public:
    render_registry();

    ~render_registry() = default;

    static render_registry& current_render()
    {
      static render_registry s_render;
      return s_render;
    }

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

    size_t register_font(const std::string& name, const mgl::registry::font_ref& font)
    {
      return m_font_manager.add_item(name, font);
    }

    void unregister_font(const std::string& name) { m_font_manager.remove_item(name); }

    bool has_font(const std::string& name) { return m_font_manager.has_item(name); }

    mgl::registry::font_ref get_font(const std::string& name)
    {
      return m_font_manager.get_item(name);
    }

    mgl::registry::font_ref get_font(size_t idx) { return m_font_manager.get_item(idx); }

private:
    shader_manager m_shader_manager;
    texture_manager m_texture_manager;
    buffer_manager m_buffer_manager;
    font_manager m_font_manager;
  };

  inline render_registry& current_render_registry()
  {
    return render_registry::current_render();
  }

} // namespace mgl::graphics