#include "mgl_opengl/compute_shader.hpp"
#include "mgl_opengl/context.hpp"

#include "mgl_opengl_internal/utils.hpp"

#include "mgl_core/debug.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  compute_shader::compute_shader(const context_ref& ctx,
                                 const std::string& source,
                                 const std::string& filename)
      : gl_object(ctx)
      , m_filename(filename)
  {
    int32_t glo = glCreateProgram();
    MGL_CORE_ASSERT(glo, "[Compute] Cannot create program.");

    int32_t shader_glo = glCreateShader(GL_COMPUTE_SHADER);
    MGL_CORE_ASSERT(shader_glo, "[Compute] Cannot create shader object.");

    const GLchar* source_str = source.c_str();
    glShaderSource(shader_glo, 1, &source_str, 0);
    glCompileShader(shader_glo);

    int32_t compiled = GL_FALSE;
    glGetShaderiv(shader_glo, GL_COMPILE_STATUS, &compiled);

    if(!compiled)
    {
      int32_t log_len = 0;
      glGetShaderiv(shader_glo, GL_INFO_LOG_LENGTH, &log_len);
      char* log = new char[log_len];
      glGetShaderInfoLog(shader_glo, log_len, &log_len, log);
      if(m_filename.size() > 0)
      {
        MGL_CORE_ERROR("[Compute] [{0}] GLSL compilation failed: {1}", m_filename.c_str(), log);
      }
      else
      {
        MGL_CORE_ERROR("[Compute] GLSL compilation failed: {0}", log);
      }
      delete[] log;
      glDeleteShader(shader_glo);
      glDeleteProgram(glo);
      MGL_CORE_ASSERT(false, "[Compute] GLSL Compilation failed.");
      return;
    }

    glAttachShader(glo, shader_glo);
    glLinkProgram(glo);

    int32_t linked = GL_FALSE;
    glGetProgramiv(glo, GL_LINK_STATUS, &linked);

    if(!linked)
    {
      int32_t log_len = 0;
      glGetProgramiv(glo, GL_INFO_LOG_LENGTH, &log_len);
      char* log = new char[log_len];
      glGetProgramInfoLog(glo, log_len, &log_len, log);
      MGL_CORE_ERROR("[Compute] GLSL link failed: {0}", log);
      delete[] log;
      glDeleteShader(shader_glo);
      glDeleteProgram(glo);
      MGL_CORE_ASSERT(false, "[Compute] GLSL Linker failed.");
      return;
    }

    m_shader_glo = shader_glo;
    gl_object::set_glo(glo);

    int32_t num_uniforms = 0;

    glGetProgramiv(glo, GL_ACTIVE_UNIFORMS, &num_uniforms);

    for(int32_t i = 0; i < num_uniforms; ++i)
    {
      int32_t type = 0;
      int32_t size = 0;
      int32_t name_len = 0;
      char name[256];

      glGetActiveUniform(glo, i, 256, &name_len, &size, (GLenum*)&type, name);
      int32_t location = glGetUniformLocation(glo, name);

      internal::clean_glsl_name(name, name_len);

      if(location < 0)
      {
        continue;
      }

      m_uniforms_map.insert(
          { name, mgl::create_ref<mgl::opengl::uniform>(name, type, glo, location, size) });
    }

    int32_t num_uniform_blocks = 0;
    glGetProgramiv(glo, GL_ACTIVE_UNIFORM_BLOCKS, &num_uniform_blocks);

    for(int32_t i = 0; i < num_uniform_blocks; ++i)
    {
      int32_t size = 0;
      int32_t name_len = 0;
      char name[256];

      glGetActiveUniformBlockName(glo, i, 256, &name_len, name);
      int32_t index = glGetUniformBlockIndex(glo, name);
      glGetActiveUniformBlockiv(glo, index, GL_UNIFORM_BLOCK_DATA_SIZE, &size);

      internal::clean_glsl_name(name, name_len);

      m_uniform_blocks_map.insert(
          { name, mgl::create_ref<mgl::opengl::uniform_block>(name, glo, index, size) });
    }
  }

  void compute_shader::release()
  {
    MGL_CORE_ASSERT(!released(), "[Compute] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Compute] Resource context not current.");
    glDeleteShader(m_shader_glo);
    glDeleteProgram(gl_object::glo());
    gl_object::set_glo(GL_ZERO);
  }

  void compute_shader::run(int32_t x, int32_t y, int32_t z, bool barrier)
  {
    MGL_CORE_ASSERT(!released(), "[Compute] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Compute] Resource context not current.");
    glUseProgram(gl_object::glo());
    glDispatchCompute(x, y, z);
    if(barrier)
    {
      glMemoryBarrier(GL_ALL_BARRIER_BITS);
    }
  }

} // namespace  mgl::opengl
