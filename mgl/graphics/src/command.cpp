#include "mgl_graphics/commands/draw.hpp"
#include "mgl_graphics/commands/functions.hpp"
#include "mgl_graphics/commands/pipeline.hpp"
#include "mgl_graphics/commands/state.hpp"
#include "mgl_graphics/commands/texture.hpp"
#include "mgl_graphics/graphics.hpp"

#include "mgl_platform/commands.hpp"
#include "mgl_platform/window.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/profiling.hpp"

#include "glm/gtc/matrix_transform.hpp"

namespace mgl::graphics
{
  command_buffer::command_buffer()
      : m_render_target(nullptr)
      , m_commands()
  {
    m_commands.reserve(100);
  }

  void command_buffer::begin()
  {
    auto vb = get_buffer("text_vb");
    MGL_CORE_ASSERT(vb != nullptr, "Font vertex buffer is null");
    vb->seek(0);
    m_commands.clear();
  }

  void command_buffer::end()
  {
    MGL_PROFILE_FUNCTION("RENDER_SCRIPT");
    if(m_render_target != nullptr)
    {
      MGL_CORE_ASSERT(false, "Render target not implemented");
      // m_render_target->use();
    }
    else
    {
      mgl::platform::gpu::commands::bind_screen_framebuffer();
    }

    for(auto& command : m_commands)
    {
      command->execute();
    }
  }

  void command_buffer::clear(const glm::vec4& color)
  {
    submit(mgl::create_ref<mgl::graphics::clear_command>(color));
  }

  void command_buffer::set_view(const glm::mat4& view)
  {
    submit(mgl::create_ref<mgl::graphics::set_view_command>(view));
  }

  void command_buffer::set_projection(const glm::mat4& projection)
  {
    submit(mgl::create_ref<mgl::graphics::set_projection_command>(projection));
  }

  void command_buffer::enable_scissor()
  {
    submit(mgl::create_ref<mgl::graphics::enable_scissor_command>());
  }

  void command_buffer::disable_scissor()
  {
    submit(mgl::create_ref<mgl::graphics::disable_scissor_command>());
  }

  void command_buffer::enable_state(int state)
  {
    submit(mgl::create_ref<mgl::graphics::enable_state>(state));
  }

  void command_buffer::disable_state(int state)
  {
    submit(mgl::create_ref<mgl::graphics::disable_state>(state));
  }

  void command_buffer::set_blend_equation(blend_equation_mode modeRGB, blend_equation_mode modeAlpha)
  {
    submit(mgl::create_ref<mgl::graphics::set_blend_equation_command>(modeRGB, modeAlpha));
  }

  void command_buffer::set_blend_func(blend_factor srcRGB,
                                     blend_factor dstRGB,
                                     blend_factor srcAlpha,
                                     blend_factor dstAlpha)
  {
    submit(
        mgl::create_ref<mgl::graphics::set_blend_func_command>(srcRGB, dstRGB, srcAlpha, dstAlpha));
  }

  void command_buffer::clear_samplers(int start, int end)
  {
    submit(mgl::create_ref<mgl::graphics::clear_samplers_command>(start, end));
  }

  void command_buffer::bind_pipeline(pipeline_ref p)
  {
    submit(mgl::create_ref<mgl::graphics::enable_pipeline>(p));
  }

  void command_buffer::bind_pipeline(const std::string& name)
  {
    auto p = get_pipeline(name);
    MGL_CORE_ASSERT(p != nullptr, "Pipeline is null");
    submit(mgl::create_ref<mgl::graphics::enable_pipeline>(p));
  }

  void command_buffer::bind_pipeline(uint32_t idx)
  {
    auto p = get_pipeline(idx);
    MGL_CORE_ASSERT(p != nullptr, "Pipeline is null");
    submit(mgl::create_ref<mgl::graphics::enable_pipeline>(p));
  }

  void command_buffer::set_uniform(const std::string& name, bool value)
  {
    submit(mgl::create_ref<mgl::graphics::set_uniform>(name, value));
  }

  void command_buffer::set_uniform(const std::string& name, int value)
  {
    submit(mgl::create_ref<mgl::graphics::set_uniform>(name, value));
  }

  void command_buffer::set_uniform(const std::string& name, float value)
  {
    submit(mgl::create_ref<mgl::graphics::set_uniform>(name, value));
  }

  void command_buffer::set_uniform(const std::string& name, const glm::vec2& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_uniform>(name, value));
  }

  void command_buffer::set_uniform(const std::string& name, const glm::vec3& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_uniform>(name, value));
  }

  void command_buffer::set_uniform(const std::string& name, const glm::vec4& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_uniform>(name, value));
  }

  void command_buffer::set_uniform(const std::string& name, const glm::mat2& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_uniform>(name, value));
  }

  void command_buffer::set_uniform(const std::string& name, const glm::mat2x3& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_uniform>(name, value));
  }

  void command_buffer::set_uniform(const std::string& name, const glm::mat2x4& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_uniform>(name, value));
  }

  void command_buffer::set_uniform(const std::string& name, const glm::mat3& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_uniform>(name, value));
  }

  void command_buffer::set_uniform(const std::string& name, const glm::mat3x2& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_uniform>(name, value));
  }

  void command_buffer::set_uniform(const std::string& name, const glm::mat3x4& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_uniform>(name, value));
  }

  void command_buffer::set_uniform(const std::string& name, const glm::mat4& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_uniform>(name, value));
  }

  void command_buffer::set_uniform(const std::string& name, const glm::mat4x2& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_uniform>(name, value));
  }

  void command_buffer::set_uniform(const std::string& name, const glm::mat4x3& value)
  {
    submit(mgl::create_ref<mgl::graphics::set_uniform>(name, value));
  }

  void command_buffer::disable_pipeline()
  {
    submit(mgl::create_ref<mgl::graphics::disable_pipeline>());
  }

  void command_buffer::bind_texture(uint32_t slot, const texture_ref& tex)
  {
    submit(mgl::create_ref<mgl::graphics::enable_texture>(slot, tex));
  }

  void command_buffer::bind_texture(uint32_t slot, const std::string& name)
  {
    auto tex = get_texture(name);
    MGL_CORE_ASSERT(tex != nullptr, "Texture is null");
    submit(mgl::create_ref<mgl::graphics::enable_texture>(slot, tex));
  }

  void command_buffer::bind_texture(uint32_t slot, uint32_t idx)
  {
    auto tex = get_texture(idx);
    MGL_CORE_ASSERT(tex != nullptr, "Texture is null");
    submit(mgl::create_ref<mgl::graphics::enable_texture>(slot, tex));
  }

  void command_buffer::draw(const mgl::platform::api::vertex_buffer_ref& vertex_array,
                           const mgl::platform::api::index_buffer_ref& index_buffer,
                           render_mode mode,
                           size_t count,
                           size_t offset)
  {
    submit(mgl::create_ref<mgl::graphics::draw_command>(
        vertex_array, index_buffer, mode, count, offset));
  }

  void command_buffer::draw(const mgl::platform::render_batch_ref& batch)
  {
    submit(mgl::create_ref<mgl::graphics::draw_batch_command>(batch));
  }

  void command_buffer::draw_text(const std::string& text,
                                const glm::vec2& position,
                                const glm::vec4& color,
                                uint32_t size,
                                const std::string& font)
  {
    auto atlas = fonts().get_atlas(font);
    MGL_CORE_ASSERT(atlas != nullptr, "Font atlas is null");
    auto tex = fonts().get_texture(font);
    MGL_CORE_ASSERT(tex != nullptr, "Font texture is null");

    auto p = get_pipeline("text_pipeline");
    MGL_CORE_ASSERT(p != nullptr, "Text pipeline is null");
    auto vb = std::static_pointer_cast<mgl::platform::api::vertex_buffer>(get_buffer("text_vb"));

    set_projection(glm::ortho(0.0f,
                              static_cast<float>(mgl::platform::current_window().width()),
                              0.0f,
                              static_cast<float>(mgl::platform::current_window().height())));
    bind_pipeline(p);
    set_uniform("color", color);
    set_uniform("px_range", static_cast<float>(atlas->pixel_height()));
    bind_texture(0, tex);
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

    disable_pipeline();
    clear_samplers(0, 1);
  }

} // namespace mgl::graphics