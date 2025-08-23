#pragma once

#include "mgl_core/containers.hpp"
#include "mgl_core/math.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"

namespace mgl::metal
{
  // Forward declarations
  class buffer;
  class texture_2d;
  class texture_3d;
  class texture_cube;
  class program;

  using buffer_ref = mgl::ref<buffer>;
  using texture_2d_ref = mgl::ref<texture_2d>;
  using texture_3d_ref = mgl::ref<texture_3d>;
  using texture_cube_ref = mgl::ref<texture_cube>;
  using program_ref = mgl::ref<program>;

  using metal_context = void*;

  struct context_mode
  {
    enum mode
    {
      DEFAULT,
      SHARE,
      ATTACHED,
      STANDALONE,
    };
  };

  class context;

  using context_ref = mgl::ref<context>;

  class context : public std::enable_shared_from_this<context>
  {
public:
    context(context_mode::mode mode, int32_t required);
    ~context();

    bool released() const { return m_released; }
    context_mode::mode mode() const { return m_mode; }

    void enter();
    void exit();

    void release();

    inline bool is_valid() const { return m_device != nullptr; }
    bool is_current() const;

    inline metal_context device() const { return m_device; }
    inline metal_context command_queue() const { return m_commandQueue; }

    // Buffer
    buffer_ref buffer(const mgl::uint8_buffer& data, bool dynamic = false)
    {
      return buffer((void*)data.data(), data.size() * sizeof(uint8_t), dynamic);
    }

    buffer_ref buffer(const mgl::uint16_buffer& data, bool dynamic = false)
    {
      return buffer((void*)data.data(), data.size() * sizeof(uint16_t), dynamic);
    }

    buffer_ref buffer(const mgl::uint32_buffer& data, bool dynamic = false)
    {
      return buffer((void*)data.data(), data.size() * sizeof(uint32_t), dynamic);
    }

    buffer_ref buffer(const mgl::int8_buffer& data, bool dynamic = false)
    {
      return buffer((void*)data.data(), data.size() * sizeof(int8_t), dynamic);
    }

    buffer_ref buffer(const mgl::int16_buffer& data, bool dynamic = false)
    {
      return buffer((void*)data.data(), data.size() * sizeof(int16_t), dynamic);
    }

    buffer_ref buffer(const mgl::int32_buffer& data, bool dynamic = false)
    {
      return buffer((void*)data.data(), data.size() * sizeof(int32_t), dynamic);
    }

    buffer_ref buffer(const mgl::float32_buffer& data, bool dynamic = false)
    {
      return buffer((void*)data.data(), data.size() * sizeof(float), dynamic);
    }

    buffer_ref buffer(const mgl::float64_buffer& data, bool dynamic = false)
    {
      return buffer((void*)data.data(), data.size() * sizeof(double), dynamic);
    }

    buffer_ref buffer(size_t reserve = 0, bool dynamic = false)
    {
      return buffer(nullptr, reserve, dynamic);
    }

    buffer_ref buffer(void* data, size_t size, bool dynamic);

    // Texture factory methods
    texture_2d_ref texture_2d(int32_t width,
                              int32_t height,
                              int32_t components = 4,
                              const void* data = nullptr,
                              int32_t samples = 0,
                              int32_t align = 1,
                              const std::string& dtype = "f1");

    texture_3d_ref texture_3d(int32_t width,
                              int32_t height,
                              int32_t depth,
                              int32_t components = 4,
                              const void* data = nullptr,
                              int32_t align = 1,
                              const std::string& dtype = "f1");

    texture_cube_ref texture_cube(int32_t size,
                                  int32_t components = 4,
                                  const void* data = nullptr,
                                  int32_t samples = 0,
                                  int32_t align = 1,
                                  const std::string& dtype = "f1");

    // Program factory method
    program_ref program(const std::string& vertex_shader,
                        const std::string& fragment_shader,
                        const std::string& geometry_shader = "",
                        const std::string& tess_evaluation_shader = "",
                        const std::string& tess_control_shader = "",
                        const mgl::dict<std::string, int>& fragment_outputs = {},
                        const mgl::string_list& varyings_names = {},
                        bool interleaved = true);

private:
    bool m_released = false;
    context_mode::mode m_mode = context_mode::DEFAULT;

    metal_context m_device = nullptr;
    metal_context m_commandQueue = nullptr;
  };

  inline context_ref create_context(context_mode::mode mode = context_mode::DEFAULT,
                                    int required = 0)
  {
    return mgl::create_ref<context>(mode, required);
  }

} // namespace mgl::metal