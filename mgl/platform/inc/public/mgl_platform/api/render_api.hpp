#pragma once

#include "buffers.hpp"
#include "enums.hpp"
#include "program.hpp"
#include "textures.hpp"

#include "mgl_registry/resources/image.hpp"

#include "glm/glm.hpp"

namespace mgl::platform::api
{
  struct draw_call
  {
    texture_ref tex;
    size_t element_count;
    size_t index_offset;
    glm::vec4 clip_rect;
  };

  struct render_batch
  {
    int32_t count;
    platform::api::render_mode render_mode;
    platform::api::vertex_buffer_ref vertex_buffer;
    platform::api::index_buffer_ref index_buffer;
    platform::api::buffer_ref uniform_buffer;
    mgl::list<draw_call> draw_calls;

    render_batch(const mgl::platform::api::vertex_buffer_ref& vb = nullptr,
                 const mgl::platform::api::index_buffer_ref& ib = nullptr,
                 const mgl::platform::api::buffer_ref& ub = nullptr,
                 platform::api::render_mode m = render_mode::TRIANGLES)
        : count(0)
        , render_mode(m)
        , vertex_buffer(vb)
        , index_buffer(ib)
        , uniform_buffer(ub)
    { }

    void clear()
    {
      draw_calls.clear();
      count = 0;
    }

    void add_draw_call(texture_ref tex,
                       size_t element_count,
                       size_t index_offset,
                       const glm::vec4& clip_rect = glm::vec4(0))
    {
      draw_calls.push_back({ tex, element_count, index_offset, clip_rect });
      count++;
    }

    ~render_batch() { clear(); }
  };

  using render_batch_ref = mgl::ref<render_batch>;

  struct render_state
  {
    render_state()
        : current_program(nullptr)
        , view_matrix(1.0f)
        , projection_matrix(1.0f)
    { }

    // The current shader, view and projection matrices are stored in the render, as they are
    // used by multiple commands
    program_ref current_program;

    // Transform matrices
    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;
  };

  class render_api;
  using render_api_ref = mgl::scope<render_api>;

  class render_api
  {
public:
    enum class dialect
    {
      OPENGL,
      METAL,
      UNKNOWN
    };

    virtual ~render_api() = default;

    virtual dialect api() const = 0;

    static render_api& instance();

    virtual bool api_init() = 0;

    virtual void api_shutdown() = 0;

    virtual void api_update_window_size(const glm::ivec2& size) = 0;

    virtual void api_bind_screen_framebuffer() = 0;

    virtual void api_enable_scissor() = 0;

    virtual void api_disable_scissor() = 0;

    virtual void api_set_scissor(const glm::vec2& position, const glm::vec2& size) = 0;

    virtual void api_enable_state(int32_t state) = 0;

    virtual void api_disable_state(int32_t state) = 0;

    virtual void api_clear(const glm::vec4& color) = 0;

    virtual void api_set_viewport(const glm::vec2& position, const glm::vec2& size) = 0;

    virtual void api_clear_samplers(int32_t start = 0, int32_t end = -1) = 0;

    virtual void api_set_blend_equation(blend_equation_mode modeRGB,
                                        blend_equation_mode modeAlpha) = 0;

    virtual void api_set_blend_func(blend_factor srcRGB,
                                    blend_factor dstRGB,
                                    blend_factor srcAlpha,
                                    blend_factor dstAlpha) = 0;

    virtual void api_set_view_matrix(const glm::mat4& matrix) = 0;

    virtual void api_set_projection_matrix(const glm::mat4& matrix) = 0;

    virtual void api_enable_program(const mgl::platform::api::program_ref& program) = 0;

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

    virtual void api_bind_texture(int32_t unit, const mgl::platform::api::texture_ref& texture) = 0;

    virtual void api_render_call(const mgl::platform::api::vertex_buffer_ref& vertex_buffer,
                                 const mgl::platform::api::index_buffer_ref& index_buffer,
                                 int32_t count,
                                 int32_t offset,
                                 render_mode mode) = 0;

    virtual void api_render_call(const mgl::platform::api::render_batch_ref& batch) = 0;

    virtual index_buffer_ref
    api_create_index_buffer(size_t size, uint16_t element_size, bool dynamic) = 0;

    virtual vertex_buffer_ref api_create_vertex_buffer(const std::string& layout,
                                                       mgl::string_list attrs,
                                                       size_t size,
                                                       bool dynamic) = 0;

    virtual buffer_ref api_create_buffer(size_t size, bool dynamic) = 0;

    virtual program_ref api_create_program(const std::string& vs_source,
                                           const std::string& fs_source,
                                           const std::string& gs_source = "",
                                           const std::string& tes_source = "",
                                           const std::string& tcs_source = "",
                                           const std::string& filename = "") = 0;

    virtual texture_2d_ref api_create_texture_2d(int32_t width,
                                                 int32_t height,
                                                 int32_t components,
                                                 int32_t samples = 0) = 0;

public:
    static bool init_api() { return render_api::instance().api_init(); }

    static void shutdown_api() { render_api::instance().api_shutdown(); }

    static void update_window_size(const glm::vec2& size)
    {
      render_api::instance().api_update_window_size(size);
    }

protected:
    render_api() = default;
  };

} // namespace mgl::platform::api