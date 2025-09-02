#pragma once

#include "mgl_platform/api/pipeline.hpp"
#include "mgl_platform/api/buffers.hpp"
#include "mgl_platform/enums.hpp"

#include "mgl_opengl/vertex_array.hpp"

namespace mgl::platform::api::backends
{
  class ogl_vertex_array;
  using ogl_vertex_array_ref = mgl::ref<ogl_vertex_array>;

  class ogl_vertex_array 
  {
public:
    ogl_vertex_array(const pipeline_ref& prg,
                     const vertex_buffer_ref& vertex_buffers,
                     const index_buffer_ref& index_buffer);

    virtual ~ogl_vertex_array() = default;

    virtual void release()
    {
      MGL_CORE_ASSERT(m_vertex_array, "Invalid vertex array");
      m_vertex_array->release();
    }

    virtual void
    render(render_mode mode, int32_t first, int32_t vertices, int32_t instances);

    virtual void render_indirect(const buffer_ref& buffer,
                                 render_mode mode,
                                 int32_t count,
                                 int32_t first);

    virtual void bind(int32_t location,
                      char type,
                      const buffer_ref& buffer,
                      const std::string& layout,
                      size_t offset,
                      int32_t stride,
                      int32_t divisor,
                      bool normalize)
    {
      MGL_CORE_ASSERT(false, "Not implemented");
    }

    virtual void update(const api::pipeline_ref& prg,
                        api::vertex_buffer_ref vertex_buffers,
                        const api::index_buffer_ref& index_buffer);

    virtual int32_t vertices() const
    {
      MGL_CORE_ASSERT(m_vertex_array, "Invalid vertex array");
      return m_vertex_array->vertices();
    }

protected:
    mgl::opengl::vertex_array_ref m_vertex_array;
  };
} // namespace mgl::platform::api::backends