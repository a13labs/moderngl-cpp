#include "mgl_graphics/commands/draw.hpp"
#include "mgl_graphics/commands/functions.hpp"
#include "mgl_graphics/commands/shader.hpp"
#include "mgl_graphics/commands/state.hpp"
#include "mgl_graphics/commands/texture.hpp"
#include "mgl_graphics/graphics.hpp"

#include "mgl_platform/api/render_api.hpp"
#include "mgl_platform/window.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/profiling.hpp"

#include "glm/gtc/matrix_transform.hpp"
namespace mgl::graphics
{
  render_script::render_script()
      : m_render_target(nullptr)
      , m_commands()
  {
    auto vb = get_buffer("text_vb");
    MGL_CORE_ASSERT(vb != nullptr, "Font vertex buffer is null");
    vb->seek(0);
    m_commands.reserve(100);
  }

  render_script::render_script(const mgl::platform::api::framebuffer_ref& target)
      : m_render_target(target)
      , m_commands()
  {
    auto vb = get_buffer("text_vb");
    MGL_CORE_ASSERT(vb != nullptr, "Font vertex buffer is null");
    vb->seek(0);
    m_commands.reserve(100);
  }

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
    auto tex = get_texture(name);
    MGL_CORE_ASSERT(tex != nullptr, "Texture is null");
    submit(mgl::create_ref<mgl::graphics::enable_texture>(slot, tex));
  }

  void render_script::enable_texture(uint32_t slot, uint32_t idx)
  {
    auto tex = get_texture(idx);
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

  void render_script::draw(const mgl::platform::api::vertex_buffer_ref& vertex_array,
                           const mgl::platform::api::index_buffer_ref& index_buffer,
                           render_mode mode,
                           size_t count,
                           size_t offset)
  {
    submit(mgl::create_ref<mgl::graphics::draw_command>(
        vertex_array, index_buffer, mode, count, offset));
  }

  void render_script::draw_batch(const batch_ref& batch)
  {
    submit(mgl::create_ref<mgl::graphics::draw_batch_command>(batch));
  }

  void render_script::enable_shader(shader_ref shader)
  {
    submit(mgl::create_ref<mgl::graphics::enable_shader>(shader));
  }

  void render_script::enable_shader(const std::string& name)
  {
    auto shader = get_shader(name);
    MGL_CORE_ASSERT(shader != nullptr, "Shader is null");
    submit(mgl::create_ref<mgl::graphics::enable_shader>(shader));
  }

  void render_script::enable_shader(uint32_t idx)
  {
    auto shader = get_shader(idx);
    MGL_CORE_ASSERT(shader != nullptr, "Shader is null");
    submit(mgl::create_ref<mgl::graphics::enable_shader>(shader));
  }

  void render_script::set_shader_uniform(const std::string& name, bool value)
  {
    submit(mgl::create_ref<mgl::graphics::set_shader_uniform>(name, value));
  }

  void render_script::set_shader_uniform(const std::string& name, int value)
  {
    submit(mgl::create_ref<mgl::graphics::set_shader_uniform>(name, value));
  }

  void render_script::set_shader_uniform(const std::string& name, float value)
  {
    submit(mgl::create_ref<mgl::graphics::set_shader_uniform>(name, value));
  }

  void render_script::set_shader_uniform(const std::string& name, const glm::vec2& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_shader_uniform>(name, value));
  }

  void render_script::set_shader_uniform(const std::string& name, const glm::vec3& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_shader_uniform>(name, value));
  }

  void render_script::set_shader_uniform(const std::string& name, const glm::vec4& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_shader_uniform>(name, value));
  }

  void render_script::set_shader_uniform(const std::string& name, const glm::mat2& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_shader_uniform>(name, value));
  }

  void render_script::set_shader_uniform(const std::string& name, const glm::mat2x3& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_shader_uniform>(name, value));
  }

  void render_script::set_shader_uniform(const std::string& name, const glm::mat2x4& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_shader_uniform>(name, value));
  }

  void render_script::set_shader_uniform(const std::string& name, const glm::mat3& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_shader_uniform>(name, value));
  }

  void render_script::set_shader_uniform(const std::string& name, const glm::mat3x2& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_shader_uniform>(name, value));
  }

  void render_script::set_shader_uniform(const std::string& name, const glm::mat3x4& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_shader_uniform>(name, value));
  }

  void render_script::set_shader_uniform(const std::string& name, const glm::mat4& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_shader_uniform>(name, value));
  }

  void render_script::set_shader_uniform(const std::string& name, const glm::mat4x2& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_shader_uniform>(name, value));
  }

  void render_script::set_shader_uniform(const std::string& name, const glm::mat4x3& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_shader_uniform>(name, value));
  }

  void render_script::disable_shader()
  {
    submit(mgl::create_ref<mgl::graphics::disable_shader>());
  }

  void render_script::execute()
  {
    MGL_PROFILE_FUNCTION("RENDER_SCRIPT");
    if(m_render_target != nullptr)
    {
      MGL_CORE_ASSERT(false, "Render target not implemented");
      // m_render_target->use();
    }
    else
    {
      mgl::platform::api::render_api::bind_screen_framebuffer();
    }

    for(auto& command : m_commands)
    {
      command->execute();
    }
  }

  void render_script::draw_text(const std::string& text,
                                const glm::vec2& position,
                                const glm::vec4& color,
                                uint32_t size,
                                const std::string& font)
  {
    auto atlas = fonts().get_atlas(font);
    MGL_CORE_ASSERT(atlas != nullptr, "Font atlas is null");
    auto tex = fonts().get_texture(font);
    MGL_CORE_ASSERT(tex != nullptr, "Font texture is null");

    auto shader = get_shader("text_shader");
    MGL_CORE_ASSERT(shader != nullptr, "Text shader is null");
    auto vb = std::static_pointer_cast<mgl::platform::api::vertex_buffer>(get_buffer("text_vb"));

    set_projection(glm::ortho(0.0f,
                              static_cast<float>(mgl::platform::current_window().width()),
                              0.0f,
                              static_cast<float>(mgl::platform::current_window().height())));
    enable_shader(shader);
    set_shader_uniform("color", color);
    set_shader_uniform("px_range", static_cast<float>(atlas->pixel_height()));
    enable_texture(0, tex);
    set_blend_func(blend_factor::SRC_ALPHA, blend_factor::ONE_MINUS_SRC_ALPHA);
    set_blend_equation(blend_equation_mode::ADD);

    // convert position to screen space
    auto x = position.x;
    auto y = mgl::platform::current_window().height() - position.y;

    int32_t vertices = 0;
    int32_t first = vb->needle() / sizeof(glm::vec4);
    float scale = static_cast<float>(size) / atlas->pixel_height();
    atlas->text_to_vertices({ x, y }, text, vb, vertices, scale, scale);
    draw(vb, nullptr, render_mode::TRIANGLES, vertices, first);

    disable_shader();
    clear_samplers(0, 1);
  }

} // namespace mgl::graphics