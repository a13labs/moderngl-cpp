#pragma once

#include "mgl_core/containers.hpp"
#include "mgl_core/string.hpp"
#include "mgl_opengl/context.hpp"

namespace mgl::platform::api
{
  // Map the opengl namespace to the api namespace, eventually we might have a vulkan api
  using context_ref = mgl::opengl::context_ref;
  using texture_ref = mgl::opengl::texture_ref;
  using texture_2d_ref = mgl::opengl::texture_2d_ref;
  using program_ref = mgl::opengl::program_ref;
  using uniform_ref = mgl::opengl::uniform_ref;
  using buffer_ref = mgl::opengl::buffer_ref;
  using texture_2d_ref = mgl::opengl::texture_2d_ref;
  using framebuffer_ref = mgl::opengl::framebuffer_ref;

  enum render_mode : uint32_t
  {
    POINTS,
    LINES,
    LINE_LOOP,
    LINE_STRIP,
    TRIANGLES,
    TRIANGLE_STRIP,
    TRIANGLE_FAN,
    LINES_ADJACENCY,
    LINE_STRIP_ADJACENCY,
    TRIANGLES_ADJACENCY,
    TRIANGLE_STRIP_ADJACENCY,
    PATCHES,
  };

  enum enable_flag : uint32_t
  {
    NOTHING = 0,
    BLEND = BIT(1),
    DEPTH_TEST = BIT(2),
    CULL_FACE = BIT(3),
    STENCIL_TEST = BIT(4),
    RASTERIZER_DISCARD = BIT(5),
    PROGRAM_POINT_SIZE = BIT(6),
    INVALID = 0x40000000
  };

  enum compare_func : uint32_t
  {
    NONE,
    NEVER,
    LESS,
    EQUAL,
    LESS_EQUAL,
    GREATER,
    NOT_EQUAL,
    GREATER_EQUAL,
    ALWAYS,
  };

  enum blend_equation_mode : uint32_t
  {
    ADD,
    SUBTRACT,
    REVERSE_SUBTRACT,
    MIN,
    MAX,
  };

  enum blend_factor : uint32_t
  {
    ZERO,
    ONE,
    SRC_COLOR,
    ONE_MINUS_SRC_COLOR,
    DST_COLOR,
    ONE_MINUS_DST_COLOR,
    SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA,
    DST_ALPHA,
    ONE_MINUS_DST_ALPHA,
    CONSTANT_COLOR,
    ONE_MINUS_CONSTANT_COLOR,
    CONSTANT_ALPHA,
    ONE_MINUS_CONSTANT_ALPHA,
    SRC_ALPHA_SATURATE,
    SRC1_COLOR,
    ONE_MINUS_SRC1_COLOR,
    SRC1_ALPHA,
    ONE_MINUS_SRC1_ALPHA,
  };

  mgl::opengl::blend_factor to_api(blend_factor factor);

  mgl::opengl::blend_equation_mode to_api(blend_equation_mode mode);

  mgl::opengl::render_mode to_api(render_mode mode);

  mgl::opengl::compare_func to_api(compare_func func);

  struct render_state
  {
    render_state()
        : current_program(nullptr)
        , view_matrix(1.0f)
        , view_uniform(nullptr)
        , projection_matrix(1.0f)
        , projection_uniform(nullptr)
        , model_uniform(nullptr)
    { }

    // The current shader, view and projection matrices are stored in the render, as they are
    // used by multiple commands
    program_ref current_program;
    mgl::string_list attributes;

    // Transform matrices
    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;

    // Program uniforms
    uniform_ref view_uniform;
    uniform_ref projection_uniform;
    uniform_ref model_uniform;
  };

  struct batch_data
  {
    batch_data()
        : model_view(1.0f)
        , count(0)
        , offset(0)
        , instance_count(1)
    { }

    batch_data(glm::mat4 t, size_t count, size_t offset, size_t instance_count)
        : model_view(t)
        , count(count)
        , offset(offset)
        , instance_count(instance_count)
    { }

    batch_data(const batch_data& other)
        : model_view(other.model_view)
        , count(other.count)
        , offset(other.offset)
        , instance_count(other.instance_count)
    { }

    glm::mat4 model_view;
    size_t count;
    size_t offset;
    size_t instance_count;
  };

  class geom_data
  {
public:
    geom_data(const buffer_ref& vb, const std::string& layout, uint32_t mode)
        : m_vb(vb)
        , m_layout(layout)
        , m_ib(nullptr)
        , m_element_size(0)
        , m_mode(mode)
        , m_vao(nullptr)
    { }

    geom_data(const buffer_ref& vb,
              const std::string& layout,
              const buffer_ref& ib,
              uint16_t element_size,
              uint32_t mode)
        : m_vb(vb)
        , m_layout(layout)
        , m_ib(ib)
        , m_element_size(element_size)
        , m_mode(mode)
        , m_vao(nullptr)
    { }

    ~geom_data();

    geom_data(const geom_data& other) = delete;

    void allocate(program_ref program = nullptr);

    void deallocate();

    void draw(const glm::mat4& model_view, int count, int offset, int instance_count);

    bool is_allocated() const { return m_vao != nullptr; }

private:
    buffer_ref m_vb = nullptr;
    std::string m_layout = "";
    buffer_ref m_ib = nullptr;
    uint16_t m_element_size = 0;
    uint32_t m_mode = 0;
    mgl::opengl::vertex_array_ref m_vao = nullptr;
  };

  void init_api();

  void shutdown_api();

  program_ref create_program(const std::string& vs_source,
                             const std::string& fs_source,
                             const std::string& gs_source = "",
                             const std::string& tes_source = "",
                             const std::string& tcs_source = "");

  texture_2d_ref create_texture_2d(
      int width, int height, int components, const mgl::uint8_buffer& data, int samples = 0);

  texture_2d_ref create_texture_2d(int width, int height, int components, int samples = 0);

  void bind_screen_framebuffer();

  void enable_scissor();

  void disable_scissor();

  void set_scissor(const glm::vec2& position, const glm::vec2& size);

  void enable_state(int state);

  void disable_state(int state);

  void clear(const glm::vec4& color);

  void set_viewport(const glm::vec2& position, const glm::vec2& size);

  void clear_samplers(int start = 0, int end = -1);

  void set_blend_equation(blend_equation_mode modeRGB, blend_equation_mode modeAlpha);

  void set_blend_func(blend_factor srcRGB,
                      blend_factor dstRGB,
                      blend_factor srcAlpha,
                      blend_factor dstAlpha);

  inline void set_scissor(const glm::vec4& rect)
  {
    set_scissor(glm::vec2(rect), glm::vec2(rect.z, rect.w));
  }

  inline void set_scissor(float x, float y, float width, float height)
  {
    set_scissor(glm::vec4(x, y, width, height));
  }

  inline void clear(float r, float g, float b, float a)
  {
    clear(glm::vec4(r, g, b, a));
  }

  inline void set_blend_equation(blend_equation_mode mode)
  {
    set_blend_equation(mode, mode);
  }

  inline void set_blend_func(blend_factor src, blend_factor dst)
  {
    set_blend_func(src, dst, src, dst);
  }

  void set_view_matrix(const glm::mat4& matrix);

  void set_projection_matrix(const glm::mat4& matrix);

  void enable_program(const program_ref& program);

  void set_program_attributes(const mgl::string_list& attributes);

  void disable_program();

  inline void
  draw(geom_data& geom, const glm::mat4& model_view, int count, int offset, int instance_count)
  {
    geom.draw(model_view, count, offset, instance_count);
  }

  void draw_batch(geom_data& geom, const mgl::list<batch_data>& data);

}; // namespace mgl::platform::api