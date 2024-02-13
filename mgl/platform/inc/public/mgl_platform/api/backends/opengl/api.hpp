#pragma once

#include "mgl_opengl/context.hpp"
#include "mgl_platform/api/render_api.hpp"

namespace mgl::platform::api::backends
{
  class opengl_api : public render_api
  {
public:
    opengl_api() { }

    virtual ~opengl_api() = default;

    mgl::opengl::context_ref& get_context() { return m_ctx; }

    virtual render_api::dialect api() const override final { return render_api::dialect::OPENGL; }

    static mgl::opengl::context_ref& current_context()
    {
      auto& api = mgl::platform::api::render_api::instance();
      MGL_CORE_ASSERT(api.api() == render_api::dialect::OPENGL, "Invalid API");
      return static_cast<mgl::platform::api::backends::opengl_api&>(api).get_context();
    }

private:
    virtual bool api_init() override final;

    virtual void api_shutdown() override final;

    virtual void api_update_window_size(const glm::ivec2& size) override final;

    virtual void api_bind_screen_framebuffer() override final;

    virtual void api_enable_scissor() override final;

    virtual void api_disable_scissor() override final;

    virtual void api_set_scissor(const glm::vec2& position, const glm::vec2& size) override final;

    virtual void api_enable_state(int32_t state) override final;

    virtual void api_disable_state(int32_t state) override final;

    virtual void api_clear(const glm::vec4& color) override final;

    virtual void api_set_viewport(const glm::vec2& position, const glm::vec2& size) override final;

    virtual void api_clear_samplers(int32_t start = 0, int32_t end = -1) override final;

    virtual void api_set_blend_equation(blend_equation_mode modeRGB,
                                        blend_equation_mode modeAlpha) override final;

    virtual void api_set_blend_func(blend_factor srcRGB,
                                    blend_factor dstRGB,
                                    blend_factor srcAlpha,
                                    blend_factor dstAlpha) override final;

    virtual void api_set_view_matrix(const glm::mat4& matrix) override final;

    virtual void api_set_projection_matrix(const glm::mat4& matrix) override final;

    virtual void api_enable_program(const program_ref& program) override final;

    virtual void api_set_program_uniform(const std::string& uniform, bool value) override final;

    virtual void api_set_program_uniform(const std::string& uniform, int32_t value) override final;

    virtual void api_set_program_uniform(const std::string& uniform, float value) override final;

    virtual void api_set_program_uniform(const std::string& uniform,
                                         const glm::vec2& value) override final;

    virtual void api_set_program_uniform(const std::string& uniform,
                                         const glm::vec3& value) override final;

    virtual void api_set_program_uniform(const std::string& uniform,
                                         const glm::vec4& value) override final;

    virtual void api_set_program_uniform(const std::string& uniform,
                                         const glm::mat2& value) override final;

    virtual void api_set_program_uniform(const std::string& uniform,
                                         const glm::mat2x3& value) override final;

    virtual void api_set_program_uniform(const std::string& uniform,
                                         const glm::mat2x4& value) override final;

    virtual void api_set_program_uniform(const std::string& uniform,
                                         const glm::mat3& value) override final;

    virtual void api_set_program_uniform(const std::string& uniform,
                                         const glm::mat3x2& value) override final;

    virtual void api_set_program_uniform(const std::string& uniform,
                                         const glm::mat3x4& value) override final;

    virtual void api_set_program_uniform(const std::string& uniform,
                                         const glm::mat4& value) override final;

    virtual void api_set_program_uniform(const std::string& uniform,
                                         const glm::mat4x2& value) override final;

    virtual void api_set_program_uniform(const std::string& uniform,
                                         const glm::mat4x3& value) override final;

    virtual void api_disable_program() override final;

    virtual void api_render_call(const mgl::platform::api::vertex_buffer_ref& vertex_buffer,
                                 const mgl::platform::api::index_buffer_ref& index_buffer,
                                 int32_t count,
                                 int32_t offset,
                                 render_mode mode) override final;

    virtual index_buffer_ref
    api_create_index_buffer(size_t size, uint16_t element_size, bool dynamic) override final;

    virtual vertex_buffer_ref
    api_create_vertex_buffer(size_t size, const std::string& layout, bool dynamic) override final;

    virtual buffer_ref api_create_buffer(size_t size, bool dynamic) override final;

    virtual program_ref api_create_program(const std::string& vs_source,
                                           const std::string& fs_source,
                                           const std::string& gs_source = "",
                                           const std::string& tes_source = "",
                                           const std::string& tcs_source = "",
                                           const std::string& filename = "") override final;

    virtual texture_2d_ref api_create_texture_2d(int32_t width,
                                                 int32_t height,
                                                 int32_t components,
                                                 int32_t samples = 0) override final;

private:
    mgl::platform::api::render_state m_state_data;
    mgl::opengl::context_ref m_ctx;
  };

} // namespace mgl::platform::api::backends
