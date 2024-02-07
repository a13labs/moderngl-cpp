#include "mgl_opengl/compute_shader.hpp"
#include "mgl_opengl/context.hpp"

#include "mgl_opengl_internal/utils.hpp"

#include "mgl_core/debug.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  compute_shader::compute_shader(const context_ref& ctx, const std::string& source)
      : gl_object(ctx)
  {
    int32_t glo = glCreateProgram();
    MGL_CORE_ASSERT(glo, "cannot create program");

    int32_t shader_glo = glCreateShader(GL_COMPUTE_SHADER);
    MGL_CORE_ASSERT(shader_glo, "cannot create shader object");

    const GLchar* source_str = source.c_str();
    glShaderSource(shader_glo, 1, &source_str, 0);
    glCompileShader(shader_glo);

    int32_t compiled = GL_FALSE;
    glGetShaderiv(shader_glo, GL_COMPILE_STATUS, &compiled);

    if(!compiled)
    {
      const char* message = "GLSL Compiler failed";
      const char* title = "ComputeShader";
      const char* underline = "=============";

      int32_t log_len = 0;
      glGetShaderiv(shader_glo, GL_INFO_LOG_LENGTH, &log_len);

      char* log = new char[log_len];
      glGetShaderInfoLog(shader_glo, log_len, &log_len, log);

      glDeleteShader(shader_glo);

      MGL_CORE_ERROR("{0}\n\n{1}\n{2}\n{3}\n", message, title, underline, log);

      delete[] log;
      MGL_CORE_ASSERT(false, "GLSL Compiler failed");
      glDeleteProgram(glo);
      return;
    }

    glAttachShader(glo, shader_glo);
    glLinkProgram(glo);

    int32_t linked = GL_FALSE;
    glGetProgramiv(glo, GL_LINK_STATUS, &linked);

    if(!linked)
    {
      const char* message = "GLSL Linker failed";
      const char* title = "ComputeShader";
      const char* underline = "=============";

      int32_t log_len = 0;
      glGetProgramiv(glo, GL_INFO_LOG_LENGTH, &log_len);

      char* log = new char[log_len];
      glGetProgramInfoLog(glo, log_len, &log_len, log);

      glDeleteProgram(glo);

      MGL_CORE_ERROR("{0}\n\n{1}\n{2}\n{3}\n", message, title, underline, log);

      delete[] log;
      glDeleteProgram(glo);
      glDeleteShader(shader_glo);
      MGL_CORE_ASSERT(false, "GLSL Linker failed");
      return;
    }

    m_shader_glo = shader_glo;
    m_glo = glo;

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

      mgl::opengl::internal::clean_glsl_name(name, name_len);

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

      mgl::opengl::internal::clean_glsl_name(name, name_len);

      m_uniform_blocks_map.insert(
          { name, mgl::create_ref<mgl::opengl::uniform_block>(name, glo, index, size) });
    }
  }

  void compute_shader::release()
  {
    MGL_CORE_ASSERT(m_glo, "Compute Shader already released or not initialized");
    glDeleteShader(m_shader_glo);
    glDeleteProgram(m_glo);
    m_glo = 0;
  }

  void compute_shader::run(int32_t x, int32_t y, int32_t z)
  {
    MGL_CORE_ASSERT(m_glo, "Compute Shader already released or not initialized");
    glUseProgram(m_glo);
    glDispatchCompute(x, y, z);
  }

  const mgl::string_list compute_shader::uniforms() const
  {
    auto result = mgl::string_list();

    for(auto&& a : m_uniforms_map)
    {
      result.push_back(a.first);
    }

    return result;
  }

  const mgl::string_list compute_shader::uniform_blocks() const
  {
    auto result = mgl::string_list();

    for(auto&& a : m_uniform_blocks_map)
    {
      result.push_back(a.first);
    }

    return result;
  }

} // namespace  mgl::opengl
