#pragma once

#include "mgl_opengl/context.hpp"
#include "mgl_platform/api/render_api.hpp"

namespace mgl::platform::api::backends
{
  class opengl_api : public render_api
  {
public:
    opengl_api()
        : render_api()
    { }

    virtual ~opengl_api() = default;

    virtual void init() override final;

    virtual void shutdown() override final;

    virtual void bind_screen_framebuffer() override final;

    virtual void enable_scissor() override final;

    virtual void disable_scissor() override final;

    virtual void set_scissor(const glm::vec2& position, const glm::vec2& size) override final;

    virtual void enable_state(int state) override final;

    virtual void disable_state(int state) override final;

    virtual void clear(const glm::vec4& color) override final;

    virtual void set_viewport(const glm::vec2& position, const glm::vec2& size) override final;

    virtual void clear_samplers(int start = 0, int end = -1) override final;

    virtual void set_blend_equation(blend_equation_mode modeRGB,
                                    blend_equation_mode modeAlpha) override final;

    virtual void set_blend_func(blend_factor srcRGB,
                                blend_factor dstRGB,
                                blend_factor srcAlpha,
                                blend_factor dstAlpha) override final;

    virtual void set_view_matrix(const glm::mat4& matrix) override final;

    virtual void set_projection_matrix(const glm::mat4& matrix) override final;

    virtual void enable_program(const program_ref& program) override final;

    virtual void set_program_attributes(const mgl::string_list& attributes) override final;

    virtual void set_program_uniform(const std::string& uniform, bool value) override final;

    virtual void set_program_uniform(const std::string& uniform, int value) override final;

    virtual void set_program_uniform(const std::string& uniform, float value) override final;

    virtual void set_program_uniform(const std::string& uniform,
                                     const glm::vec2& value) override final;

    virtual void set_program_uniform(const std::string& uniform,
                                     const glm::vec3& value) override final;

    virtual void set_program_uniform(const std::string& uniform,
                                     const glm::vec4& value) override final;

    virtual void set_program_uniform(const std::string& uniform,
                                     const glm::mat2& value) override final;

    virtual void set_program_uniform(const std::string& uniform,
                                     const glm::mat2x3& value) override final;

    virtual void set_program_uniform(const std::string& uniform,
                                     const glm::mat2x4& value) override final;

    virtual void set_program_uniform(const std::string& uniform,
                                     const glm::mat3& value) override final;

    virtual void set_program_uniform(const std::string& uniform,
                                     const glm::mat3x2& value) override final;

    virtual void set_program_uniform(const std::string& uniform,
                                     const glm::mat3x4& value) override final;

    virtual void set_program_uniform(const std::string& uniform,
                                     const glm::mat4& value) override final;

    virtual void set_program_uniform(const std::string& uniform,
                                     const glm::mat4x2& value) override final;

    virtual void set_program_uniform(const std::string& uniform,
                                     const glm::mat4x3& value) override final;

    virtual void disable_program() override final;

    virtual void draw_batch(vertex_array_ref& geom,
                            const mgl::list<batch_data>& data) override final;

    virtual program_ref create_program(const std::string& vs_source,
                                       const std::string& fs_source,
                                       const std::string& gs_source = "",
                                       const std::string& tes_source = "",
                                       const std::string& tcs_source = "",
                                       const std::string& filename = "") override final;

    virtual texture_2d_ref create_texture_2d(int width,
                                             int height,
                                             int components,
                                             const mgl::uint8_buffer& data,
                                             int samples = 0) override final;

    virtual texture_2d_ref
    create_texture_2d(int width, int height, int components, int samples = 0) override final;

    virtual vertex_array_ref create_vertex_array(const buffer_ref& vb,
                                                 const std::string& layout,
                                                 uint32_t mode) override final;

    virtual vertex_array_ref create_vertex_array(const buffer_ref& vb,
                                                 const std::string& layout,
                                                 const buffer_ref& ib,
                                                 uint16_t element_size,
                                                 uint32_t mode) override final;
  };

  class vertex_array : public mgl::platform::api::vertex_array
  {
public:
    vertex_array(const buffer_ref& vb, const std::string& layout, uint32_t mode)
        : mgl::platform::api::vertex_array(vb, layout, mode)
        , m_vao(nullptr)
    { }

    vertex_array(const buffer_ref& vb,
                 const std::string& layout,
                 const buffer_ref& ib,
                 uint16_t element_size,
                 uint32_t mode)
        : mgl::platform::api::vertex_array(vb, layout, ib, element_size, mode)
        , m_vao(nullptr)
    { }

    virtual ~vertex_array();

    vertex_array(const vertex_array& other) = delete;

    void allocate(program_ref program = nullptr);

    void deallocate();

    void draw(const glm::mat4& model_view, int count, int offset, int instance_count);

    bool is_allocated() const { return m_vao != nullptr; }

private:
    mgl::opengl::vertex_array_ref m_vao = nullptr;
  };

} // namespace mgl::platform::api::backends
