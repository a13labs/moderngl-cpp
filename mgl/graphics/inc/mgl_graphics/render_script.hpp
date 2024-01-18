#pragma once
#include "buffers/index.hpp"
#include "buffers/vertex.hpp"
#include "enums.hpp"
#include "glm/glm.hpp"
#include "material.hpp"
#include "shader.hpp"
#include "texture.hpp"

#include "mgl_window/api.hpp"

#include "mgl_core/containers.hpp"
#include "mgl_core/memory.hpp"

namespace mgl::graphics
{
  class render_command;
  class render_script;

  using render_command_ref = mgl::ref<render_command>;
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

    void reset() { m_commands.clear(); }

    void enable_state(int state);

    void enable_state(state state) { enable_state(static_cast<int>(state)); }

    void disable_state(int state);

    void enable_texture(uint32_t slot, const texture_ref& tex);

    void enable_texture(uint32_t slot, const std::string& name);

    void enable_texture(uint32_t slot, uint32_t idx);

    void clear(const glm::vec4& color);

    void clear(float r, float g, float b, float a) { clear(glm::vec4(r, g, b, a)); }

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

    void enable_shader(const std::string& name);

    void enable_shader(uint32_t idx);

    void disable_shader();

    void enable_material(material_ref shader);

    void disable_material();

    void execute();

private:
    void submit(const render_command_ref& command) { m_commands.push_back(command); }

    mgl::list<render_command_ref> m_commands;
    mgl::window::api::framebuffer_ref m_render_target;
  };

} // namespace mgl::graphics