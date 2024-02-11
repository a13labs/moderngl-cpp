#include "mgl_platform/api/backends/opengl/program.hpp"
#include "mgl_platform/api/backends/opengl/api.hpp"

#include "mgl_core/debug.hpp"

namespace mgl::platform::api::backends::opengl
{

  program::program(const std::string& vs_source,
                   const std::string& fs_source,
                   const std::string& gs_source,
                   const std::string& tes_source,
                   const std::string& tcs_source,
                   const std::string& filename)
  {
    mgl::opengl::shaders glsl(vs_source, fs_source, gs_source, tes_source, tcs_source);
    mgl::opengl::context_ref& ctx = mgl::platform::api::backends::opengl_api::current_context();
    mgl::opengl::program_ref program = ctx->program(glsl, filename);
    m_program = program;
    MGL_CORE_ASSERT(m_program, "Failed to create program");
  }

} // namespace mgl::platform::api::backends::opengl
