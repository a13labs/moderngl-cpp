#include "mgl_platform/commands.hpp"
#include "mgl_platform/api/render_api.hpp"

namespace mgl::platform::gpu
{
  namespace
  {
    class default_command_executor : public commands
    {

      private:   
      void cmd_clear(const glm::vec4& color) override { 
        api::render_api::instance().api_clear(color); 
      }

      void cmd_bind_screen_framebuffer() override
      {
        api::render_api::instance().api_bind_screen_framebuffer();
      }

      void cmd_set_viewport(const glm::vec2& position, const glm::vec2& size) override
      {
        api::render_api::instance().api_set_viewport(position, size);
      }

      void cmd_set_view_matrix(const glm::mat4& matrix) override
      {
        api::render_api::instance().api_set_view_matrix(matrix);
      }

      void cmd_set_projection_matrix(const glm::mat4& matrix) override
      {
        api::render_api::instance().api_set_projection_matrix(matrix);
      }

      void cmd_enable_scissor() override { api::render_api::instance().api_enable_scissor(); }
      void cmd_disable_scissor() override { api::render_api::instance().api_disable_scissor(); }
      void cmd_set_scissor(const glm::vec2& position, const glm::vec2& size) override
      {
        api::render_api::instance().api_set_scissor(position, size);
      }

      void cmd_enable_state(int32_t state) override { api::render_api::instance().api_enable_state(state); }
      void cmd_disable_state(int32_t state) override { api::render_api::instance().api_disable_state(state); }

      void cmd_set_blend_equation(api::blend_equation_mode modeRGB, api::blend_equation_mode modeAlpha) override
      {
        api::render_api::instance().api_set_blend_equation(modeRGB, modeAlpha);
      }

      void cmd_set_blend_func(api::blend_factor srcRGB,
                         api::blend_factor dstRGB,
                         api::blend_factor srcAlpha,
                         api::blend_factor dstAlpha) override
      {
        api::render_api::instance().api_set_blend_func(srcRGB, dstRGB, srcAlpha, dstAlpha);
      }

      void cmd_clear_samplers(int32_t start = 0, int32_t end = -1) override
      {
        api::render_api::instance().api_clear_samplers(start, end);
      }

      void cmd_enable_pipeline(const api::program_ref& program) override
      {
        api::render_api::instance().api_enable_program(program);
      }

      void cmd_disable_pipeline() override { api::render_api::instance().api_disable_program(); }

      void cmd_set_program_uniform(const std::string& uniform, bool value) override
      {
        api::render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, int32_t value) override
      {
        api::render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, float value) override
      {
        api::render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::vec2& value) override
      {
        api::render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::vec3& value) override
      {
        api::render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::vec4& value) override
      {
        api::render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::mat2& value) override
      {
        api::render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::mat2x3& value) override
      {
        api::render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::mat2x4& value) override
      {
        api::render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::mat3& value) override
      {
        api::render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::mat3x2& value) override
      {
        api::render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::mat3x4& value) override
      {
        api::render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::mat4& value) override
      {
        api::render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::mat4x2& value) override
      {
        api::render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_set_program_uniform(const std::string& uniform, const glm::mat4x3& value) override
      {
        api::render_api::instance().api_set_program_uniform(uniform, value);
      }

      void cmd_bind_texture(int32_t unit, const api::texture_ref& texture) override
      {
        api::render_api::instance().api_bind_texture(unit, texture);
      }

      void cmd_render_call(const api::vertex_buffer_ref& vertex_buffer,
                       const api::index_buffer_ref& index_buffer,
                       int32_t count,
                       int32_t offset,
                       api::render_mode mode) override
      {
        api::render_api::instance().api_render_call(vertex_buffer, index_buffer, count, offset, mode);
      }

      void cmd_render_call(const api::render_batch_ref& batch) override
      {
        api::render_api::instance().api_render_call(batch);
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
