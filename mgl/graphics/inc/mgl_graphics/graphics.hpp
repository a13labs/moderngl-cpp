#pragma once
#include "batch.hpp"
#include "command.hpp"
#include "enums.hpp"
#include "glm/glm.hpp"
#include "managers/buffer.hpp"
#include "managers/font.hpp"
#include "managers/shader.hpp"
#include "managers/texture.hpp"
#include "shader.hpp"
#include "texture.hpp"

namespace mgl::graphics
{
  void init();

  void shutdown();

  inline size_t register_shader(const std::string& name, const shader_ref& shader)
  {
    return shaders().add_item(name, shader);
  }

  inline void unregister_shader(const std::string& name)
  {
    shaders().remove_item(name);
  }

  inline bool has_shader(const std::string& name)
  {
    return shaders().has_item(name);
  }

  inline shader_ref get_shader(const std::string& name)
  {
    return shaders().get_item(name);
  }

  inline shader_ref get_shader(size_t idx)
  {
    return shaders().get_item(idx);
  }

  inline size_t register_texture(const std::string& name, const texture_ref& texture)
  {
    return textures().add_item(name, texture);
  }

  inline void unregister_texture(const std::string& name)
  {
    textures().remove_item(name);
  }

  inline bool has_texture(const std::string& name)
  {
    return textures().has_item(name);
  }

  inline texture_ref get_texture(const std::string& name)
  {
    return textures().get_item(name);
  }

  inline texture_ref get_texture(size_t idx)
  {
    return textures().get_item(idx);
  }

  inline size_t register_buffer(const std::string& name,
                                const mgl::platform::api::buffer_ref& buffer)
  {
    return buffers().add_item(name, buffer);
  }

  inline void unregister_buffer(const std::string& name)
  {
    buffers().remove_item(name);
  }

  inline bool has_buffer(const std::string& name)
  {
    return buffers().has_item(name);
  }

  inline mgl::platform::api::buffer_ref get_buffer(const std::string& name)
  {
    return buffers().get_item(name);
  }

  inline mgl::platform::api::buffer_ref get_buffer(size_t idx)
  {
    return buffers().get_item(idx);
  }

  inline size_t register_font(const std::string& name, const mgl::registry::font_ref& font)
  {
    return fonts().add_item(name, font);
  }

  inline void unregister_font(const std::string& name)
  {
    fonts().remove_item(name);
  }

  inline bool has_font(const std::string& name)
  {
    return fonts().has_item(name);
  }

  inline mgl::registry::font_ref get_font(const std::string& name)
  {
    return fonts().get_item(name);
  }

  inline mgl::registry::font_ref get_font(size_t idx)
  {
    return fonts().get_item(idx);
  }

} // namespace mgl::graphics