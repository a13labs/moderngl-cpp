#pragma once

#include "buffers.hpp"
#include "enums.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/memory.hpp"

namespace mgl::platform::api
{
  class vertex_array;
  using vertex_array_ref = mgl::ref<vertex_array>;

  class vertex_array
  {

public:
    virtual ~vertex_array() = default;

    virtual void release() = 0;

    virtual void render(render_mode mode = render_mode::TRIANGLES,
                        int32_t first = 0,
                        int32_t vertices = 0,
                        int32_t instances = 1) = 0;

    virtual void render_indirect(const buffer_ref& buffer,
                                 render_mode mode,
                                 int32_t count = 0,
                                 int32_t first = 0) = 0;

    virtual void bind(int32_t location,
                      char type,
                      const buffer_ref& buffer,
                      const std::string& layout,
                      size_t offset = 0,
                      int32_t stride = 0,
                      int32_t divisor = 0,
                      bool normalize = false) = 0;

    virtual void update(const program_ref& prg,
                        vertex_buffer_ref vertex_buffers,
                        const index_buffer_ref& index_buffer = nullptr) = 0;

    virtual int32_t vertices() const = 0;
  };
} // namespace mgl::platform::api