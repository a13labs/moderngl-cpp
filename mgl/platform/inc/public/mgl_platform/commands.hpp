#pragma once

#include "mgl_platform/device.hpp"

namespace mgl::platform::gpu
{
  class commands
  {
public:
    virtual ~commands() = default;

private:
    // Basic commands
    virtual void impl_begin_frame() = 0;
    virtual void impl_end_frame() = 0;
    
    virtual void impl_begin_render_pass() = 0;
    virtual void impl_end_render_pass() = 0;

    virtual void impl_clear(const glm::vec4& color) = 0;
    virtual void impl_set_viewport(const glm::vec2& position, const glm::vec2& size) = 0;
    virtual void impl_set_view_matrix(const glm::mat4& matrix) = 0;
    virtual void impl_set_projection_matrix(const glm::mat4& matrix) = 0;
    virtual void impl_bind_screen_framebuffer() = 0;

    // Scissor
    virtual void impl_enable_scissor() = 0;
    virtual void impl_disable_scissor() = 0;
    virtual void impl_set_scissor(const glm::vec2& position, const glm::vec2& size) = 0;

    // State
    virtual void impl_enable_state(int32_t state) = 0;
    virtual void impl_disable_state(int32_t state) = 0;

    // Blend
    virtual void impl_set_blend_equation(api::blend_equation_mode modeRGB,
                                        api::blend_equation_mode modeAlpha) = 0;
    virtual void impl_set_blend_func(api::blend_factor srcRGB,
                                    api::blend_factor dstRGB,
                                    api::blend_factor srcAlpha,
                                    api::blend_factor dstAlpha) = 0;

    // Samplers
    virtual void impl_clear_samplers(int32_t start = 0, int32_t end = -1) = 0;

    // Pipeline
    virtual void impl_enable_pipeline(const api::pipeline_ref& program) = 0;
    virtual void impl_disable_pipeline() = 0;

    // Program uniforms (overloads)
    virtual void impl_set_uniform(const std::string& uniform, bool value) = 0;
    virtual void impl_set_uniform(const std::string& uniform, int32_t value) = 0;
    virtual void impl_set_uniform(const std::string& uniform, float value) = 0;
    virtual void impl_set_uniform(const std::string& uniform, const glm::vec2& value) = 0;
    virtual void impl_set_uniform(const std::string& uniform, const glm::vec3& value) = 0;
    virtual void impl_set_uniform(const std::string& uniform, const glm::vec4& value) = 0;
    virtual void impl_set_uniform(const std::string& uniform, const glm::mat2& value) = 0;
    virtual void impl_set_uniform(const std::string& uniform, const glm::mat2x3& value) = 0;
    virtual void impl_set_uniform(const std::string& uniform, const glm::mat2x4& value) = 0;
    virtual void impl_set_uniform(const std::string& uniform, const glm::mat3& value) = 0;
    virtual void impl_set_uniform(const std::string& uniform, const glm::mat3x2& value) = 0;
    virtual void impl_set_uniform(const std::string& uniform, const glm::mat3x4& value) = 0;
    virtual void impl_set_uniform(const std::string& uniform, const glm::mat4& value) = 0;
    virtual void impl_set_uniform(const std::string& uniform, const glm::mat4x2& value) = 0;
    virtual void impl_set_uniform(const std::string& uniform, const glm::mat4x3& value) = 0;

    // Textures
    virtual void impl_bind_texture(int32_t unit, const api::texture_ref& texture) = 0;

    // Render calls
    virtual void impl_draw(const api::vertex_buffer_ref& vertex_buffer,
                                 const api::index_buffer_ref& index_buffer,
                                 int32_t count,
                                 int32_t offset,
                                 api::render_mode mode) = 0;

    virtual void impl_draw(const render_batch_ref& batch) = 0;

public:
    static void begin_frame() { instance().impl_begin_frame(); }

    static void end_frame() { instance().impl_end_frame(); }

    static void begin_render_pass() { instance().impl_begin_render_pass(); }

    static void end_render_pass() { instance().impl_end_render_pass(); }

    static void clear(float r, float g, float b, float a)
    {
      instance().impl_clear(glm::vec4(r, g, b, a));
    }

    static void clear(const glm::vec4& color) { instance().impl_clear(color); }

    static void set_viewport(const glm::vec2& position, const glm::vec2& size)
    {
      instance().impl_set_viewport(position, size);
    }

    static void set_view_matrix(const glm::mat4& matrix) { instance().impl_set_view_matrix(matrix); }

    static void set_projection_matrix(const glm::mat4& matrix)
    {
      instance().impl_set_projection_matrix(matrix);
    }

    static void bind_screen_framebuffer() { instance().impl_bind_screen_framebuffer(); }

    // Scissor
    static void enable_scissor() { instance().impl_enable_scissor(); }

    static void disable_scissor() { instance().impl_disable_scissor(); }

    static void set_scissor(const glm::vec4& rect)
    {
      set_scissor(glm::vec2(rect), glm::vec2(rect.z, rect.w));
    }

    static void set_scissor(float x, float y, float width, float height)
    {
      set_scissor(glm::vec2(x, y), glm::vec2(width, height));
    }

    static void set_scissor(const glm::vec2& position, const glm::vec2& size)
    {
      instance().impl_set_scissor(position, size);
    }

    // State
    static void enable_state(int32_t state) { instance().impl_enable_state(state); }

    static void disable_state(int32_t state) { instance().impl_disable_state(state); }

    // Blend
    static void set_blend_equation(api::blend_equation_mode mode) { set_blend_equation(mode, mode); }

    static void set_blend_equation(api::blend_equation_mode modeRGB, api::blend_equation_mode modeAlpha)
    {
      instance().impl_set_blend_equation(modeRGB, modeAlpha);
    }

    static void set_blend_func(api::blend_factor src, api::blend_factor dst)
    {
      set_blend_func(src, dst, src, dst);
    }

    static void set_blend_func(api::blend_factor srcRGB,
                               api::blend_factor dstRGB,
                               api::blend_factor srcAlpha,
                               api::blend_factor dstAlpha)
    {
      instance().impl_set_blend_func(srcRGB, dstRGB, srcAlpha, dstAlpha);
    }

    // Samplers
    static void clear_samplers(int32_t start = 0, int32_t end = -1)
    {
      instance().impl_clear_samplers(start, end);
    }

    // Pipeline
    static void enable_pipeline(const api::pipeline_ref& program)
    {
      instance().impl_enable_pipeline(program);
    }

    static void disable_pipeline() { instance().impl_disable_pipeline(); }

    // Program uniforms (overloads)
    static void set_program_uniform(const std::string& uniform, bool value)
    {
      instance().impl_set_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, int32_t value)
    {
      instance().impl_set_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, float value)
    {
      instance().impl_set_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::vec2& value)
    {
      instance().impl_set_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::vec3& value)
    {
      instance().impl_set_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::vec4& value)
    {
      instance().impl_set_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::mat2& value)
    {
      instance().impl_set_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::mat2x3& value)
    {
      instance().impl_set_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::mat2x4& value)
    {
      instance().impl_set_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::mat3& value)
    {
      instance().impl_set_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::mat3x2& value)
    {
      instance().impl_set_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::mat3x4& value)
    {
      instance().impl_set_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::mat4& value)
    {
      instance().impl_set_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::mat4x2& value)
    {
      instance().impl_set_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::mat4x3& value)
    {
      instance().impl_set_uniform(uniform, value);
    }

    // Textures
    static void bind_texture(int32_t unit, const api::texture_ref& texture)
    {
      instance().impl_bind_texture(unit, texture);
    }

    // Render calls
    static void draw(const mgl::platform::api::vertex_buffer_ref& vertex_buffer,
                            int32_t count,
                            int32_t offset,
                            api::render_mode mode)
    {
      draw(vertex_buffer, nullptr, count, offset, mode);
    }

    static void draw(const api::vertex_buffer_ref& vertex_buffer,
                            const api::index_buffer_ref& index_buffer,
                            int32_t count,
                            int32_t offset,
                            api::render_mode mode)
    {
      instance().impl_draw(vertex_buffer, index_buffer, count, offset, mode);
    }

    static void draw(const render_batch_ref& batch) { instance().impl_draw(batch); }

    // Singleton installation
    static commands& instance();
    static void set_instance(std::unique_ptr<commands> executor);
  };

} // namespace mgl::platform::api
