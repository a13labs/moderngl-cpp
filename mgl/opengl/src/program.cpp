#include "mgl_opengl/program.hpp"
#include "mgl_opengl/context.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/string.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{

  inline void clean_glsl_name(char* name, int32_t& name_len)
  {
    if(name_len && name[name_len - 1] == ']')
    {
      name_len -= 1;
      while(name_len && name[name_len] != '[')
      {
        name_len -= 1;
      }
    }
    name[name_len] = 0;
  }

  program::program(const shaders& shaders,
                   const shaders_outputs& outputs,
                   const fragment_outputs& fragment_outputs,
                   bool interleaved)
  {
    static const int32_t SHADER_TYPE[5] = {
      GL_VERTEX_SHADER,       GL_FRAGMENT_SHADER,        GL_GEOMETRY_SHADER,
      GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER,
    };

    static const char* SHADER_NAME[] = {
      "vertex_shader",       "fragment_shader",        "geometry_shader",
      "tess_control_shader", "tess_evaluation_shader",
    };

    m_transform = shaders.sources[shader::type::FRAGMENT_SHADER].empty();

    int32_t glo = glCreateProgram();
    MGL_CORE_ASSERT(glo, "cannot create program");

    int32_t shader_objs[] = { 0, 0, 0, 0, 0 };

    for(int32_t i = 0; i < shader::type::GENERIC_PROGRAM; ++i)
    {
      if(shaders.sources[i] == "")
      {
        continue;
      }

      const char* source_str = shaders.sources[i].c_str();

      int32_t shader_glo = glCreateShader(SHADER_TYPE[i]);
      MGL_CORE_ASSERT(shader_glo, "cannot create shader object");

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
        glDeleteShader(shader_glo);

        MGL_CORE_ERROR("GLSL Compiler failed ({0}):{1}", SHADER_NAME[i], log);

        delete[] log;
        glDeleteProgram(glo);
        MGL_CORE_ASSERT(false, "GLSL Compiler failed");
        return;
      }

      shader_objs[i] = shader_glo;
      glAttachShader(glo, shader_glo);
    }

    if(outputs.size())
    {
      const char** varyings_array = new const char*[outputs.size()];

      for(int32_t i = 0; i < (int32_t)outputs.size(); ++i)
      {
        varyings_array[i] = outputs[i].c_str();
      }

      int32_t capture_mode = interleaved ? GL_INTERLEAVED_ATTRIBS : GL_SEPARATE_ATTRIBS;

      glTransformFeedbackVaryings(glo, outputs.size(), varyings_array, capture_mode);

      delete[] varyings_array;
    }

    for(auto&& fo : fragment_outputs)
    {
      glBindFragDataLocation(glo, fo.second, fo.first.c_str());
    }

    glLinkProgram(glo);

    for(int32_t i = 0; i < shader::type::GENERIC_PROGRAM; ++i)
    {
      if(shader_objs[i])
      {
        glDeleteShader(shader_objs[i]);
      }
    }

    int32_t linked = GL_FALSE;
    glGetProgramiv(glo, GL_LINK_STATUS, &linked);

    if(!linked)
    {
      const char* message = "GLSL Linker failed";
      const char* title = "Program";
      const char* underline = "=======";

      int32_t log_len = 0;
      glGetProgramiv(glo, GL_INFO_LOG_LENGTH, &log_len);

      char* log = new char[log_len];
      glGetProgramInfoLog(glo, log_len, &log_len, log);

      glDeleteProgram(glo);

      MGL_CORE_ERROR("{0}\n\n{1}\n{2}\n{3}\n", message, title, underline, log);

      delete[] log;
      glDeleteProgram(glo);
      MGL_CORE_ASSERT(false, "GLSL Linker failed");
      return;
    }

    m_glo = glo;

    int32_t num_vertex_shader_subroutines = 0;
    int32_t num_fragment_shader_subroutines = 0;
    int32_t num_geometry_shader_subroutines = 0;
    int32_t num_tess_evaluation_shader_subroutines = 0;
    int32_t num_tess_control_shader_subroutines = 0;

    int32_t num_vertex_shader_subroutine_uniforms = 0;
    int32_t num_fragment_shader_subroutine_uniforms = 0;
    int32_t num_geometry_shader_subroutine_uniforms = 0;
    int32_t num_tess_evaluation_shader_subroutine_uniforms = 0;
    int32_t num_tess_control_shader_subroutine_uniforms = 0;

    int32_t major = 0;
    int32_t minor = 0;

    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    int32_t version_code = major * 100 + minor * 10;

    if(version_code >= 400)
    {
      if(!shaders.sources[shader::type::VERTEX_SHADER].empty())
      {
        glGetProgramStageiv(
            glo, GL_VERTEX_SHADER, GL_ACTIVE_SUBROUTINES, &num_vertex_shader_subroutines);
        glGetProgramStageiv(glo,
                            GL_VERTEX_SHADER,
                            GL_ACTIVE_SUBROUTINE_UNIFORMS,
                            &num_vertex_shader_subroutine_uniforms);
      }

      if(!shaders.sources[shader::type::FRAGMENT_SHADER].empty())
      {
        glGetProgramStageiv(
            glo, GL_FRAGMENT_SHADER, GL_ACTIVE_SUBROUTINES, &num_fragment_shader_subroutines);
        glGetProgramStageiv(glo,
                            GL_FRAGMENT_SHADER,
                            GL_ACTIVE_SUBROUTINE_UNIFORMS,
                            &num_fragment_shader_subroutine_uniforms);
      }

      if(!shaders.sources[shader::type::GEOMETRY_SHADER].empty())
      {
        glGetProgramStageiv(
            glo, GL_GEOMETRY_SHADER, GL_ACTIVE_SUBROUTINES, &num_geometry_shader_subroutines);
        glGetProgramStageiv(glo,
                            GL_GEOMETRY_SHADER,
                            GL_ACTIVE_SUBROUTINE_UNIFORMS,
                            &num_geometry_shader_subroutine_uniforms);
      }

      if(!shaders.sources[shader::type::TESS_EVALUATION_SHADER].empty())
      {
        glGetProgramStageiv(glo,
                            GL_TESS_EVALUATION_SHADER,
                            GL_ACTIVE_SUBROUTINES,
                            &num_tess_evaluation_shader_subroutines);
        glGetProgramStageiv(glo,
                            GL_TESS_EVALUATION_SHADER,
                            GL_ACTIVE_SUBROUTINE_UNIFORMS,
                            &num_tess_evaluation_shader_subroutine_uniforms);
      }

      if(!shaders.sources[shader::type::TESS_CONTROL_SHADER].empty())
      {
        glGetProgramStageiv(glo,
                            GL_TESS_CONTROL_SHADER,
                            GL_ACTIVE_SUBROUTINES,
                            &num_tess_control_shader_subroutines);
        glGetProgramStageiv(glo,
                            GL_TESS_CONTROL_SHADER,
                            GL_ACTIVE_SUBROUTINE_UNIFORMS,
                            &num_tess_control_shader_subroutine_uniforms);
      }
    }

    m_num_vertex_shader_subroutines = num_vertex_shader_subroutine_uniforms;
    m_num_fragment_shader_subroutines = num_fragment_shader_subroutine_uniforms;
    m_num_geometry_shader_subroutines = num_geometry_shader_subroutine_uniforms;
    m_num_tess_evaluation_shader_subroutines = num_tess_evaluation_shader_subroutine_uniforms;
    m_num_tess_control_shader_subroutines = num_tess_control_shader_subroutine_uniforms;

    if(!shaders.sources[shader::type::GEOMETRY_SHADER].empty())
    {

      int32_t geometry_in = 0;
      int32_t geometry_out = 0;
      m_geometry_vertices = 0;

      glGetProgramiv(glo, GL_GEOMETRY_INPUT_TYPE, &geometry_in);
      glGetProgramiv(glo, GL_GEOMETRY_OUTPUT_TYPE, &geometry_out);
      glGetProgramiv(glo, GL_GEOMETRY_VERTICES_OUT, &m_geometry_vertices);

      switch(geometry_in)
      {
        case GL_TRIANGLES: m_geometry_input = GL_TRIANGLES; break;

        case GL_TRIANGLE_STRIP: m_geometry_input = GL_TRIANGLE_STRIP; break;

        case GL_TRIANGLE_FAN: m_geometry_input = GL_TRIANGLE_FAN; break;

        case GL_LINES: m_geometry_input = GL_LINES; break;

        case GL_LINE_STRIP: m_geometry_input = GL_LINE_STRIP; break;

        case GL_LINE_LOOP: m_geometry_input = GL_LINE_LOOP; break;

        case GL_POINTS: m_geometry_input = GL_POINTS; break;

        case GL_LINE_STRIP_ADJACENCY: m_geometry_input = GL_LINE_STRIP_ADJACENCY; break;

        case GL_LINES_ADJACENCY: m_geometry_input = GL_LINES_ADJACENCY; break;

        case GL_TRIANGLE_STRIP_ADJACENCY: m_geometry_input = GL_TRIANGLE_STRIP_ADJACENCY; break;

        case GL_TRIANGLES_ADJACENCY: m_geometry_input = GL_TRIANGLES_ADJACENCY; break;

        default: m_geometry_input = -1; break;
      }

      switch(geometry_out)
      {
        case GL_TRIANGLES: m_geometry_output = GL_TRIANGLES; break;

        case GL_TRIANGLE_STRIP: m_geometry_output = GL_TRIANGLES; break;

        case GL_TRIANGLE_FAN: m_geometry_output = GL_TRIANGLES; break;

        case GL_LINES: m_geometry_output = GL_LINES; break;

        case GL_LINE_STRIP: m_geometry_output = GL_LINES; break;

        case GL_LINE_LOOP: m_geometry_output = GL_LINES; break;

        case GL_POINTS: m_geometry_output = GL_POINTS; break;

        case GL_LINE_STRIP_ADJACENCY: m_geometry_output = GL_LINES; break;

        case GL_LINES_ADJACENCY: m_geometry_output = GL_LINES; break;

        case GL_TRIANGLE_STRIP_ADJACENCY: m_geometry_output = GL_TRIANGLES; break;

        case GL_TRIANGLES_ADJACENCY: m_geometry_output = GL_TRIANGLES; break;

        default: m_geometry_output = -1; break;
      }
    }
    else
    {
      m_geometry_input = -1;
      m_geometry_output = -1;
      m_geometry_vertices = 0;
    }

    int32_t num_attributes = 0;
    int32_t num_varyings = 0;
    int32_t num_uniforms = 0;
    int32_t num_uniform_blocks = 0;

    glGetProgramiv(m_glo, GL_ACTIVE_ATTRIBUTES, &num_attributes);
    glGetProgramiv(m_glo, GL_TRANSFORM_FEEDBACK_VARYINGS, &num_varyings);
    glGetProgramiv(m_glo, GL_ACTIVE_UNIFORMS, &num_uniforms);
    glGetProgramiv(m_glo, GL_ACTIVE_UNIFORM_BLOCKS, &num_uniform_blocks);

    for(int32_t i = 0; i < num_attributes; ++i)
    {
      int32_t type = 0;
      int32_t array_length = 0;
      int32_t name_len = 0;
      char name[256];

      glGetActiveAttrib(m_glo, i, 256, &name_len, &array_length, (GLenum*)&type, name);
      int32_t location = glGetAttribLocation(m_glo, name);

      clean_glsl_name(name, name_len);

      m_attributes_map.insert(
          { name,
            mgl::create_ref<mgl::opengl::attribute>(name, type, m_glo, location, array_length) });
    }

    for(int32_t i = 0; i < num_varyings; ++i)
    {
      int32_t type = 0;
      int32_t array_length = 0;
      int32_t dimension = 0;
      int32_t name_len = 0;
      char name[256];

      glGetTransformFeedbackVarying(m_glo, i, 256, &name_len, &array_length, (GLenum*)&type, name);

      m_varyings_map.insert(
          { name, mgl::create_ref<mgl::opengl::varying>(name, i, array_length, dimension) });
    }

    for(int32_t i = 0; i < num_uniforms; ++i)
    {
      int32_t type = 0;
      int32_t size = 0;
      int32_t name_len = 0;
      char name[256];

      glGetActiveUniform(m_glo, i, 256, &name_len, &size, (GLenum*)&type, name);
      int32_t location = glGetUniformLocation(m_glo, name);

      clean_glsl_name(name, name_len);

      if(location < 0)
      {
        continue;
      }

      m_uniforms_map.insert(
          { name, mgl::create_ref<mgl::opengl::uniform>(name, type, glo, location, size) });
    }

    for(int32_t i = 0; i < num_uniform_blocks; ++i)
    {
      int32_t size = 0;
      int32_t name_len = 0;
      char name[256];

      glGetActiveUniformBlockName(m_glo, i, 256, &name_len, name);
      int32_t index = glGetUniformBlockIndex(m_glo, name);
      glGetActiveUniformBlockiv(m_glo, index, GL_UNIFORM_BLOCK_DATA_SIZE, &size);

      clean_glsl_name(name, name_len);

      m_uniform_blocks_map.insert(
          { name, mgl::create_ref<mgl::opengl::uniform_block>(name, glo, index, size) });
    }

    if(version_code >= 400)
    {
      for(int32_t st = 0; st < 5; ++st)
      {
        int32_t num_subroutines = 0;
        auto type = mgl::opengl::subroutine::type(SHADER_TYPE[st]);

        glGetProgramStageiv(glo, type, GL_ACTIVE_SUBROUTINES, &num_subroutines);

        int32_t num_subroutine_uniforms = 0;
        glGetProgramStageiv(glo, type, GL_ACTIVE_SUBROUTINE_UNIFORMS, &num_subroutine_uniforms);

        for(int32_t i = 0; i < num_subroutines; ++i)
        {
          int32_t name_len = 0;
          char name[256];

          glGetActiveSubroutineName(glo, type, i, 256, &name_len, name);
          int32_t index = glGetSubroutineIndex(glo, type, name);

          m_subroutines_map.insert(
              { name, mgl::create_ref<mgl::opengl::subroutine>(name, index, type) });
        }
      }
    }
  }

  void program::release()
  {
    MGL_CORE_ASSERT(m_glo != 0, "Vertex Array already released");
    glDeleteProgram(m_glo);
    m_glo = 0;
  }

  const mgl::string_list program::attributes(bool all)
  {
    auto result = mgl::string_list();

    for(auto&& a : m_attributes_map)
    {
      if(!all && mgl::starts_with(a.first, "gl_"))
      {
        continue;
      }
      result.push_back(a.first);
    }

    return result;
  }

  const mgl::string_list program::uniforms()
  {
    auto result = mgl::string_list();

    for(auto&& a : m_uniforms_map)
    {
      result.push_back(a.first);
    }

    return result;
  }

  const mgl::string_list program::uniform_blocks()
  {
    auto result = mgl::string_list();

    for(auto&& a : m_uniform_blocks_map)
    {
      result.push_back(a.first);
    }

    return result;
  }

  const mgl::string_list program::varyings()
  {
    auto result = mgl::string_list();

    for(auto&& a : m_varyings_map)
    {
      result.push_back(a.first);
    }

    return result;
  }

  const mgl::string_list program::subroutines()
  {
    auto result = mgl::string_list();

    for(auto&& a : m_subroutines_map)
    {
      result.push_back(a.first);
    }

    return result;
  }

  void program::bind()
  {
    MGL_CORE_ASSERT(m_glo != 0, "Vertex Array already released");
    glUseProgram(m_glo);
  }

  void program::unbind()
  {
    MGL_CORE_ASSERT(m_glo != 0, "Vertex Array already released");
    glUseProgram(0);
  }

} // namespace  mgl::opengl
