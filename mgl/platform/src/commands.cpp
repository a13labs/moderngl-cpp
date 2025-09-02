#include "mgl_platform/commands.hpp"
#include "mgl_platform/device.hpp"

namespace mgl::platform::gpu
{
  namespace
  {
    class default_command_executor : public commands
    {
  private:
      void impl_begin_frame() override { device::instance().api_begin_frame(); }
      void impl_end_frame() override { device::instance().api_end_frame(); }
      void impl_begin_render_pass() override { device::instance().api_begin_render_pass(); }
      void impl_end_render_pass() override { device::instance().api_end_render_pass(); }
      
      void impl_clear(const glm::vec4& color) override
      {
        device::instance().api_clear(color);
      }

      void impl_bind_screen_framebuffer() override
      {
        device::instance().api_bind_screen_framebuffer();
      }

      void impl_set_viewport(const glm::vec2& position, const glm::vec2& size) override
      {
        device::instance().api_set_viewport(position, size);
      }

      void impl_set_view_matrix(const glm::mat4& matrix) override
      {
        device::instance().api_set_view_matrix(matrix);
      }

      void impl_set_projection_matrix(const glm::mat4& matrix) override
      {
        device::instance().api_set_projection_matrix(matrix);
      }

      void impl_enable_scissor() override { device::instance().api_enable_scissor(); }
      void impl_disable_scissor() override { device::instance().api_disable_scissor(); }
      void impl_set_scissor(const glm::vec2& position, const glm::vec2& size) override
      {
        device::instance().api_set_scissor(position, size);
      }

      void impl_enable_state(int32_t state) override
      {
        device::instance().api_enable_state(state);
      }
      void impl_disable_state(int32_t state) override
      {
        device::instance().api_disable_state(state);
      }

      void impl_set_blend_equation(api::blend_equation_mode modeRGB,
                                   api::blend_equation_mode modeAlpha) override
      {
        device::instance().api_set_blend_equation(modeRGB, modeAlpha);
      }

      void impl_set_blend_func(api::blend_factor srcRGB,
                               api::blend_factor dstRGB,
                               api::blend_factor srcAlpha,
                               api::blend_factor dstAlpha) override
      {
        device::instance().api_set_blend_func(srcRGB, dstRGB, srcAlpha, dstAlpha);
      }

      void impl_clear_samplers(int32_t start = 0, int32_t end = -1) override
      {
        device::instance().api_clear_samplers(start, end);
      }

      void impl_enable_pipeline(const api::pipeline_ref& program) override
      {
        device::instance().api_enable_program(program);
      }

      void impl_disable_pipeline() override { device::instance().api_disable_program(); }

      void impl_set_uniform(const std::string& uniform, bool value) override
      {
        device::instance().api_set_program_uniform(uniform, value);
      }

      void impl_set_uniform(const std::string& uniform, int32_t value) override
      {
        device::instance().api_set_program_uniform(uniform, value);
      }

      void impl_set_uniform(const std::string& uniform, float value) override
      {
        device::instance().api_set_program_uniform(uniform, value);
      }

      void impl_set_uniform(const std::string& uniform, const glm::vec2& value) override
      {
        device::instance().api_set_program_uniform(uniform, value);
      }

      void impl_set_uniform(const std::string& uniform, const glm::vec3& value) override
      {
        device::instance().api_set_program_uniform(uniform, value);
      }

      void impl_set_uniform(const std::string& uniform, const glm::vec4& value) override
      {
        device::instance().api_set_program_uniform(uniform, value);
      }

      void impl_set_uniform(const std::string& uniform, const glm::mat2& value) override
      {
        device::instance().api_set_program_uniform(uniform, value);
      }

      void impl_set_uniform(const std::string& uniform, const glm::mat2x3& value) override
      {
        device::instance().api_set_program_uniform(uniform, value);
      }

      void impl_set_uniform(const std::string& uniform, const glm::mat2x4& value) override
      {
        device::instance().api_set_program_uniform(uniform, value);
      }

      void impl_set_uniform(const std::string& uniform, const glm::mat3& value) override
      {
        device::instance().api_set_program_uniform(uniform, value);
      }

      void impl_set_uniform(const std::string& uniform, const glm::mat3x2& value) override
      {
        device::instance().api_set_program_uniform(uniform, value);
      }

      void impl_set_uniform(const std::string& uniform, const glm::mat3x4& value) override
      {
        device::instance().api_set_program_uniform(uniform, value);
      }

      void impl_set_uniform(const std::string& uniform, const glm::mat4& value) override
      {
        device::instance().api_set_program_uniform(uniform, value);
      }

      void impl_set_uniform(const std::string& uniform, const glm::mat4x2& value) override
      {
        device::instance().api_set_program_uniform(uniform, value);
      }

      void impl_set_uniform(const std::string& uniform, const glm::mat4x3& value) override
      {
        device::instance().api_set_program_uniform(uniform, value);
      }

      void impl_bind_texture(int32_t unit, const api::texture_ref& texture) override
      {
        device::instance().api_bind_texture(unit, texture);
      }

      void impl_draw(const api::vertex_buffer_ref& vertex_buffer,
                            const api::index_buffer_ref& index_buffer,
                            int32_t count,
                            int32_t offset,
                            api::render_mode mode) override
      {
        device::instance().api_render_call(
            vertex_buffer, index_buffer, count, offset, mode);
      }

      void impl_draw(const render_batch_ref& batch) override
      {
        device::instance().api_render_call(batch);
      }
    };

    static std::unique_ptr<commands> g_executor = nullptr;
  } // namespace

  commands& commands::instance()
  {
    if(!g_executor)
    {
      g_executor = std::make_unique<default_command_executor>();
    }
    return *g_executor;
  }

  void commands::set_instance(std::unique_ptr<commands> executor)
  {
    g_executor = std::move(executor);
  }

} // namespace mgl::platform::gpu
