#include "mgl_platform/api/opengl/vertex_array.hpp"
#include "mgl_platform/api/opengl/api.hpp"
#include "mgl_platform/api/opengl/buffers.hpp"
#include "mgl_platform/api/opengl/program.hpp"

#include "mgl_platform_internal.hpp"

#include "mgl_opengl/context.hpp"

namespace mgl::platform::api::backends
{
  ogl_vertex_array::ogl_vertex_array(const program_ref& prg,
                                     const vertex_buffer_ref& vbo,
                                     const index_buffer_ref& ibo)
  {
    MGL_CORE_ASSERT(vbo, "Invalid vertex buffer")
    MGL_CORE_ASSERT(prg, "Invalid program");

    mgl::opengl::context_ref& ctx = ogl_api::current_context();
    auto o_prg = std::static_pointer_cast<ogl_program>(prg);
    auto o_vbo = std::static_pointer_cast<ogl_vertex_buffer>(vbo);

    if(ibo)
    {
      auto o_ibo = std::static_pointer_cast<ogl_index_buffer>(ibo);
      m_vertex_array = ctx->vertex_array(
          o_prg->native(), { o_vbo->native_vbo() }, o_ibo->native(), o_ibo->element_size());
    }
    else
    {
      m_vertex_array = ctx->vertex_array(o_prg->native(), { o_vbo->native_vbo() });
    }
  }

  void
  ogl_vertex_array::render(render_mode mode, int32_t first, int32_t vertices, int32_t instances)
  {
    m_vertex_array->render(internal::to_api(mode), first, vertices, instances);
  }

  void ogl_vertex_array::render_indirect(const buffer_ref& buffer,
                                         render_mode mode,
                                         int32_t count,
                                         int32_t first)
  {
    MGL_CORE_ASSERT(false, "Not implemented");
  }

  void ogl_vertex_array::update(const program_ref& prg,
                                vertex_buffer_ref vbo,
                                const index_buffer_ref& ibo)
  {
    MGL_CORE_ASSERT(m_vertex_array, "Invalid vertex array");
    MGL_CORE_ASSERT(prg, "Invalid program");
    MGL_CORE_ASSERT(vbo, "Invalid vertex buffer")

    auto o_prg = std::static_pointer_cast<ogl_program>(prg);
    auto o_vbo = std::static_pointer_cast<ogl_vertex_buffer>(vbo);

    if(ibo)
    {
      auto o_ibo = std::static_pointer_cast<ogl_index_buffer>(ibo);
      m_vertex_array->update(
          o_prg->native(), { o_vbo->native_vbo() }, o_ibo->native(), o_ibo->element_size());
    }
    else
    {
      m_vertex_array->update(o_prg->native(), { o_vbo->native_vbo() });
    }
  }

} // namespace mgl::platform::api::backends