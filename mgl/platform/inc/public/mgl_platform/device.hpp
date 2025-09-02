#pragma once

#include "api/buffers.hpp"
#include "api/pipeline.hpp"
#include "api/textures.hpp"

#include "render.hpp"
#include "enums.hpp"

namespace mgl::platform
{
  class device;
  using device_ref = mgl::scope<device>;

  class device
  {
public:
    enum class dialect
    {
      OPENGL,
      METAL,
      UNKNOWN
    };

    virtual ~device() = default;

    virtual dialect api() const = 0;

    static device& instance();

    virtual bool api_init() = 0;

    virtual void api_shutdown() = 0;

    virtual void api_update_window_size(const glm::ivec2& size) = 0;

    // Commands

    virtual void api_begin_frame() = 0;

    virtual void api_end_frame() = 0;

    virtual void api_begin_render_pass() = 0;

    virtual void api_end_render_pass() = 0;

    virtual void api_clear(const glm::vec4& color) = 0;

    virtual void api_bind_screen_framebuffer() = 0;
    
    virtual void api_set_viewport(const glm::vec2& position, const glm::vec2& size) = 0;
    
    virtual void api_set_view_matrix(const glm::mat4& matrix) = 0;

    virtual void api_set_projection_matrix(const glm::mat4& matrix) = 0;
    
    virtual void api_enable_scissor() = 0;

    virtual void api_disable_scissor() = 0;

    virtual void api_set_scissor(const glm::vec2& position, const glm::vec2& size) = 0;

    virtual void api_enable_state(int32_t state) = 0;

    virtual void api_disable_state(int32_t state) = 0;

    virtual void api_set_blend_equation(blend_equation_mode modeRGB,
                                        blend_equation_mode modeAlpha) = 0;

    virtual void api_set_blend_func(blend_factor srcRGB,
                                    blend_factor dstRGB,
                                    blend_factor srcAlpha,
                                    blend_factor dstAlpha) = 0;

    virtual void api_clear_samplers(int32_t start = 0, int32_t end = -1) = 0;


    virtual void api_enable_program(const api::pipeline_ref& program) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, bool value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, int32_t value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, float value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::vec2& value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::vec3& value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::vec4& value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::mat2& value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::mat2x3& value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::mat2x4& value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::mat3& value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::mat3x2& value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::mat3x4& value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::mat4& value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::mat4x2& value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::mat4x3& value) = 0;

    virtual void api_disable_program() = 0;

    virtual void api_bind_texture(int32_t unit, const api::texture_ref& texture) = 0;

    virtual void api_render_call(const api::vertex_buffer_ref& vertex_buffer,
                                 const api::index_buffer_ref& index_buffer,
                                 int32_t count,
                                 int32_t offset,
                                 render_mode  mode) = 0;

    virtual void api_render_call(const render_batch_ref& batch) = 0;

    // GPU resources

    virtual api::index_buffer_ref
    api_create_index_buffer(size_t size, uint16_t element_size, bool dynamic) = 0;

    virtual api::vertex_buffer_ref api_create_vertex_buffer(const std::string& layout,
                                                           mgl::string_list attrs,
                                                           size_t size,
                                                       bool dynamic) = 0;

    virtual api::buffer_ref api_create_buffer(size_t size, bool dynamic) = 0;

    virtual api::pipeline_ref api_create_program(const std::string& vs_source,
                                           const std::string& fs_source,
                                           const std::string& gs_source = "",
                                           const std::string& tes_source = "",
                                           const std::string& tcs_source = "",
                                           const std::string& filename = "") = 0;

    virtual api::texture_2d_ref api_create_texture_2d(int32_t width,
                                                 int32_t height,
                                                 int32_t components,
                                                 int32_t samples = 0) = 0;

protected:
    device() = default;
  };


    inline static bool init_device() { return device::instance().api_init(); }

    inline static void shutdown_device() { device::instance().api_shutdown(); }

    inline static void update_window_size(const glm::vec2& size)
    {
      device::instance().api_update_window_size(size);
    }

} // namespace mgl::platform::api