#include "mgl_core/debug.hpp"
#include "mgl_graphics/commands/draw.hpp"
#include "mgl_graphics/commands/functions.hpp"
#include "mgl_graphics/commands/shader.hpp"
#include "mgl_graphics/commands/state.hpp"
#include "mgl_graphics/commands/texture.hpp"
#include "mgl_graphics/render.hpp"
namespace mgl::graphics
{

  void render_script::enable_state(int state)
  {
    submit(mgl::create_ref<mgl::graphics::enable_state>(state));
  }

  void render_script::disable_state(int state)
  {
    submit(mgl::create_ref<mgl::graphics::disable_state>(state));
  }

  void render_script::enable_texture(uint32_t slot, const texture_ref& tex)
  {
    submit(mgl::create_ref<mgl::graphics::enable_texture>(slot, tex));
  }

  void render_script::enable_texture(uint32_t slot, const std::string& name)
  {
    auto& render = mgl::graphics::current_render();
    auto tex = render.get_texture(name);
    MGL_CORE_ASSERT(tex != nullptr, "Texture is null");
    submit(mgl::create_ref<mgl::graphics::enable_texture>(slot, tex));
  }

  void render_script::enable_texture(uint32_t slot, uint32_t idx)
  {
    auto& render = mgl::graphics::current_render();
    auto tex = render.get_texture(idx);
    MGL_CORE_ASSERT(tex != nullptr, "Texture is null");
    submit(mgl::create_ref<mgl::graphics::enable_texture>(slot, tex));
  }

  void render_script::clear(const glm::vec4& color)
  {
    submit(mgl::create_ref<mgl::graphics::clear_command>(color));
  }

  void render_script::set_view(const glm::mat4& view)
  {
    submit(mgl::create_ref<mgl::graphics::set_view_command>(view));
  }

  void render_script::set_projection(const glm::mat4& projection)
  {
    submit(mgl::create_ref<mgl::graphics::set_projection_command>(projection));
  }

  void render_script::set_blend_func(blend_factor srcRGB,
                                     blend_factor dstRGB,
                                     blend_factor srcAlpha,
                                     blend_factor dstAlpha)
  {
    submit(
        mgl::create_ref<mgl::graphics::set_blend_func_command>(srcRGB, dstRGB, srcAlpha, dstAlpha));
  }

  void render_script::clear_samplers(int start, int end)
  {
    submit(mgl::create_ref<mgl::graphics::clear_samplers_command>(start, end));
  }

  void render_script::set_blend_equation(blend_equation_mode modeRGB, blend_equation_mode modeAlpha)
  {
    submit(mgl::create_ref<mgl::graphics::set_blend_equation_command>(modeRGB, modeAlpha));
  }

  void render_script::draw(const vertex_buffer_ref& vertex_array,
                           const index_buffer_ref& index_buffer,
                           render_mode mode,
                           glm::mat4 transform,
                           size_t count,
                           size_t offset)
  {
    submit(mgl::create_ref<mgl::graphics::draw_command>(
        vertex_array, index_buffer, mode, transform, count, offset));
  }

  void render_script::draw_batch(const batch_render_ref& batch)
  {
    submit(mgl::create_ref<mgl::graphics::draw_batch_command>(batch));
  }

  void render_script::enable_shader(shader_ref shader)
  {
    submit(mgl::create_ref<mgl::graphics::enable_shader>(shader));
  }

  void render_script::enable_shader(const std::string& name)
  {
    auto& render = mgl::graphics::current_render();
    auto shader = render.get_shader(name);
    MGL_CORE_ASSERT(shader != nullptr, "Shader is null");
    submit(mgl::create_ref<mgl::graphics::enable_shader>(shader));
  }

  void render_script::enable_shader(uint32_t idx)
  {
    auto& render = mgl::graphics::current_render();
    auto shader = render.get_shader(idx);
    MGL_CORE_ASSERT(shader != nullptr, "Shader is null");
    submit(mgl::create_ref<mgl::graphics::enable_shader>(shader));
  }

  void render_script::disable_shader()
  {
    submit(mgl::create_ref<mgl::graphics::disable_shader>());
  }

  void render_script::execute()
  {
    auto& render = mgl::graphics::current_render();
    auto ctx = render.context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");

    if(m_render_target != nullptr)
    {
      m_render_target->use();
    }
    else
    {
      ctx->screen()->use();
    }

    for(auto& command : m_commands)
    {
      command->execute();
    }

    ctx->screen()->use();
  }

} // namespace mgl::graphics