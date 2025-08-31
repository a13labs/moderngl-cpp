#include "mgl_platform/api/commands.hpp"
#include "mgl_platform/api/render_api.hpp"

namespace mgl::platform::api
{
  namespace
  {
    class default_command_executor : public commands
    {

      private:   
      void cmd_clear(const glm::vec4& color) override { 
        render_api::instance().api_clear(color); 
      }

      void cmd_bind_screen_framebuffer() override
      {
        render_api::instance().api_bind_screen_framebuffer();
      }

      void cmd_set_viewport(const glm::vec2& position, const glm::vec2& size) override
      {
        render_api::instance().api_set_viewport(position, size);
      }

      void cmd_set_view_matrix(const glm::mat4& matrix) override
      {
        render_api::instance().api_set_view_matrix(matrix);
      }

      void cmd_set_projection_matrix(const glm::mat4& matrix) override
      {
        render_api::instance().api_set_projection_matrix(matrix);
      }

      void cmd_enable_scissor() override { render_api::instance().api_enable_scissor(); }
      void cmd_disable_scissor() override { render_api::instance().api_disable_scissor(); }
      void cmd_set_scissor(const glm::vec2& position, const glm::vec2& size) override
      {
        render_api::instance().api_set_scissor(position, size);
      }

      void cmd_enable_state(int32_t state) override { render_api::instance().api_enable_state(state); }
      void cmd_disable_state(int32_t state) override { render_api::instance().api_disable_state(state); }

      void cmd_set_blend_equation(blend_equation_mode modeRGB, blend_equation_mode modeAlpha) override
      {
        render_api::instance().api_set_blend_equation(modeRGB, modeAlpha);
      }

      void cmd_set_blend_func(blend_factor srcRGB,
                         blend_factor dstRGB,
                         blend_factor srcAlpha,
                         blend_factor dstAlpha) override
      {
        render_api::instance().api_set_blend_func(srcRGB, dstRGB, srcAlpha, dstAlpha);
      }

      void cmd_clear_samplers(int32_t start = 0, int32_t end = -1) override
      {
        render_api::instance().api_clear_samplers(start, end);
      }

      void cmd_enable_pipeline(const program_ref& program) override
      {
        render_api::instance().api_enable_program(program);
      }

      void cmd_disable_pipeline() override { render_api::instance().api_disable_program(); }

      void cmd_set_program_uniform(const std::string& uniform, bool value) override
      {
        render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, int32_t value) override
      {
        render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, float value) override
      {
        render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::vec2& value) override
      {
        render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::vec3& value) override
      {
        render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::vec4& value) override
      {
        render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::mat2& value) override
      {
        render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::mat2x3& value) override
      {
        render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::mat2x4& value) override
      {
        render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::mat3& value) override
      {
        render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::mat3x2& value) override
      {
        render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::mat3x4& value) override
      {
        render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::mat4& value) override
      {
        render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::mat4x2& value) override
      {
        render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::mat4x3& value) override
      {
        render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_bind_texture(int32_t unit, const texture_ref& texture) override
      {
        render_api::instance().api_bind_texture(unit, texture);
      }

      void cmd_render_call(const vertex_buffer_ref& vertex_buffer,
                       const index_buffer_ref& index_buffer,
                       int32_t count,
                       int32_t offset,
                       render_mode mode) override
      {
        render_api::instance().api_render_call(vertex_buffer, index_buffer, count, offset, mode);
      }

      void cmd_render_call(const render_batch_ref& batch) override
      {
        render_api::instance().api_render_call(batch);
      }
    };

    static std::unique_ptr<commands> g_executor = nullptr;
  } // namespace

  commands& commands::instance()
  {
    if (!g_executor)
    {
      g_executor = std::make_unique<default_command_executor>();
    }
    return *g_executor;
  }

  void commands::set_instance(std::unique_ptr<commands> executor)
  {
    g_executor = std::move(executor);
  }

} // namespace mgl::platform::api
