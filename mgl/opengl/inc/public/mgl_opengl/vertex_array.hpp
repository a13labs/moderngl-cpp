#pragma once
#include "buffer.hpp"
#include "enums.hpp"
#include "gl_object.hpp"
#include "program.hpp"
#include "vertex_buffer.hpp"

#include "mgl_core/containers.hpp"
#include "mgl_core/memory.hpp"

namespace mgl::opengl
{
  class vertex_array : public gl_object
  {
public:
    typedef struct
    {
      uint32_t count;
      uint32_t instance_count;
      uint32_t first;
      uint32_t base_instance;
    } draw_indirect_command;

    ~vertex_array() = default;

    virtual void release() override;

    void render(render_mode mode = render_mode::TRIANGLES,
                int32_t first = 0,
                int32_t vertices = 0,
                int32_t instances = 1);

    void render(int32_t instances) { render(mgl::opengl::TRIANGLES, 0, 0, instances); }

    void render_indirect(const buffer_ref& buffer,
                         render_mode mode,
                         int32_t count = 0,
                         int32_t first = 0);

    void transform(const buffer_ref& b,
                   render_mode mode,
                   int32_t vertices = 0,
                   int32_t first = 0,
                   int32_t instances = 1)
    {
      transform(mgl::ref_list<buffer>({ b }), mode, vertices, first, instances);
    }

    void transform(const mgl::ref_list<buffer>& buffers,
                   render_mode mode,
                   int32_t vertices = 0,
                   int32_t first = 0,
                   int32_t instances = 1,
                   int32_t buffer_offset = 0);

    void bind(int32_t location,
              char type,
              const buffer_ref& buffer,
              const buffer_layout& layout,
              size_t offset = 0,
              int32_t stride = 0,
              int32_t divisor = 0,
              bool normalize = false);

    void update(const program_ref& prg,
                mgl::opengl::vertex_buffer_list vertex_buffers,
                const buffer_ref& index_buffer = nullptr,
                int32_t element_size = 4);

    void update(const program_ref& prg,
                mgl::opengl::vertex_buffer vertex_buffer,
                const buffer_ref& index_buffer = nullptr,
                int32_t element_size = 4)
    {
      update(prg, mgl::opengl::vertex_buffer_list({ vertex_buffer }), index_buffer, element_size);
    }

    int32_t vertices() const { return m_num_vertices; }

private:
    friend class context;

    vertex_array(const context_ref& ctx,
                 const program_ref& prg,
                 mgl::opengl::vertex_buffer_list vertex_buffers,
                 buffer_ref index_buffer,
                 int32_t element_size);

    program_ref m_prg;
    buffer_ref m_ibo;
    int32_t m_element_size;
    int32_t m_element_type;
    int32_t m_num_vertices;
  };

  using vertex_array_ref = mgl::ref<vertex_array>;

} // namespace  mgl::opengl
