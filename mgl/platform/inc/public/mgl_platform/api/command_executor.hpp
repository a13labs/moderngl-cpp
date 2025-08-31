#pragma once

#include "mgl_platform/api/render_api.hpp"

#include <memory>

namespace mgl::platform::api
{
  class command_executor
  {
  public:
    virtual ~command_executor() = default;

    // Basic commands
    void clear(float r, float g, float b, float a) {
      clear(glm::vec4(r, g, b, a));
    }
    virtual void clear(const glm::vec4& color) = 0;
    virtual void set_viewport(const glm::vec2& position, const glm::vec2& size) = 0;
    virtual void set_view_matrix(const glm::mat4& matrix) = 0;
    virtual void set_projection_matrix(const glm::mat4& matrix) = 0;

    // Scissor
    virtual void enable_scissor() = 0;
    virtual void disable_scissor() = 0;

    void set_scissor(const glm::vec4& rect)
    {
      set_scissor(glm::vec2(rect), glm::vec2(rect.z, rect.w));
    }

    void set_scissor(float x, float y, float width, float height)
    {
      set_scissor(glm::vec2(x, y), glm::vec2(width, height));
    }

    virtual void set_scissor(const glm::vec2& position, const glm::vec2& size) = 0;

    // State
    virtual void enable_state(int32_t state) = 0;
    virtual void disable_state(int32_t state) = 0;

    // Blend
    void set_blend_equation(blend_equation_mode mode)
    {
      set_blend_equation(mode, mode);
    }

    virtual void set_blend_equation(blend_equation_mode modeRGB, blend_equation_mode modeAlpha) = 0;

    void set_blend_func(blend_factor src, blend_factor dst)
    {
      set_blend_func(src, dst, src, dst);
    }

    virtual void set_blend_func(blend_factor srcRGB,
                                blend_factor dstRGB,
                                blend_factor srcAlpha,
                                blend_factor dstAlpha) = 0;

    // Samplers
    virtual void clear_samplers(int32_t start = 0, int32_t end = -1) = 0;

    // Program / shader
    virtual void enable_pipeline(const program_ref& program) = 0;
    virtual void disable_pipeline() = 0;

    // Program uniforms (overloads)
    virtual void set_program_uniform(const std::string& uniform, bool value) = 0;
    virtual void set_program_uniform(const std::string& uniform, int32_t value) = 0;
    virtual void set_program_uniform(const std::string& uniform, float value) = 0;
    virtual void set_program_uniform(const std::string& uniform, const glm::vec2& value) = 0;
    virtual void set_program_uniform(const std::string& uniform, const glm::vec3& value) = 0;
    virtual void set_program_uniform(const std::string& uniform, const glm::vec4& value) = 0;
    virtual void set_program_uniform(const std::string& uniform, const glm::mat2& value) = 0;
    virtual void set_program_uniform(const std::string& uniform, const glm::mat2x3& value) = 0;
    virtual void set_program_uniform(const std::string& uniform, const glm::mat2x4& value) = 0;
    virtual void set_program_uniform(const std::string& uniform, const glm::mat3& value) = 0;
    virtual void set_program_uniform(const std::string& uniform, const glm::mat3x2& value) = 0;
    virtual void set_program_uniform(const std::string& uniform, const glm::mat3x4& value) = 0;
    virtual void set_program_uniform(const std::string& uniform, const glm::mat4& value) = 0;
    virtual void set_program_uniform(const std::string& uniform, const glm::mat4x2& value) = 0;
    virtual void set_program_uniform(const std::string& uniform, const glm::mat4x3& value) = 0;

    // Textures
    virtual void bind_texture(int32_t unit, const texture_ref& texture) = 0;

    // Render calls
    void render_call(const mgl::platform::api::vertex_buffer_ref& vertex_buffer,
                            int32_t count,
                            int32_t offset,
                            render_mode mode)
    {
      render_call(vertex_buffer, nullptr, count, offset, mode);
    }

    virtual void render_call(const vertex_buffer_ref& vertex_buffer,
                             const index_buffer_ref& index_buffer,
                             int32_t count,
                             int32_t offset,
                             render_mode mode) = 0;

    virtual void render_call(const render_batch_ref& batch) = 0;

    // Singleton installation
    static command_executor& instance();
    static void set_instance(std::unique_ptr<command_executor> executor);
  };

} // namespace mgl::platform::api
