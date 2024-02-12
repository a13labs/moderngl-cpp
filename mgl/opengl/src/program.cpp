#include "mgl_opengl/program.hpp"
#include "mgl_opengl/context.hpp"

#include "mgl_opengl_internal.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/string.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  static program::data_type gl_int = { 1, GL_INT, 1, 1, false };
  static program::data_type gl_int_vec2 = { 2, GL_INT, 1, 2, false };
  static program::data_type gl_int_vec3 = { 3, GL_INT, 1, 3, false };
  static program::data_type gl_int_vec4 = { 4, GL_INT, 1, 4, false };
  static program::data_type gl_unsigned_int = { 1, GL_UNSIGNED_INT, 1, 1, false };
  static program::data_type gl_unsigned_int_vec2 = { 2, GL_UNSIGNED_INT, 1, 2, false };
  static program::data_type gl_unsigned_int_vec3 = { 3, GL_UNSIGNED_INT, 1, 3, false };
  static program::data_type gl_unsigned_int_vec4 = { 4, GL_UNSIGNED_INT, 1, 4, false };
  static program::data_type gl_float = { 1, GL_FLOAT, 1, 1, true };
  static program::data_type gl_float_vec2 = { 2, GL_FLOAT, 1, 2, true };
  static program::data_type gl_float_vec3 = { 3, GL_FLOAT, 1, 3, true };
  static program::data_type gl_float_vec4 = { 4, GL_FLOAT, 1, 4, true };
  static program::data_type gl_double = { 1, GL_DOUBLE, 1, 1, false };
  static program::data_type gl_double_vec2 = { 2, GL_DOUBLE, 1, 2, false };
  static program::data_type gl_double_vec3 = { 3, GL_DOUBLE, 1, 3, false };
  static program::data_type gl_double_vec4 = { 4, GL_DOUBLE, 1, 4, false };
  static program::data_type gl_float_mat_2 = { 4, GL_FLOAT, 2, 2, true };
  static program::data_type gl_float_mat_2x3 = { 6, GL_FLOAT, 2, 3, true };
  static program::data_type gl_float_mat_2x4 = { 8, GL_FLOAT, 2, 4, true };
  static program::data_type gl_float_mat_3x2 = { 6, GL_FLOAT, 3, 2, true };
  static program::data_type gl_float_mat_3 = { 9, GL_FLOAT, 3, 3, true };
  static program::data_type gl_float_mat_3x4 = { 12, GL_FLOAT, 3, 4, true };
  static program::data_type gl_float_mat_4x2 = { 8, GL_FLOAT, 4, 2, true };
  static program::data_type gl_float_mat_4x3 = { 12, GL_FLOAT, 4, 3, true };
  static program::data_type gl_float_mat_4 = { 16, GL_FLOAT, 4, 4, true };
  static program::data_type gl_double_mat_2 = { 4, GL_DOUBLE, 2, 2, false };
  static program::data_type gl_double_mat_2x3 = { 6, GL_DOUBLE, 2, 3, false };
  static program::data_type gl_double_mat_2x4 = { 8, GL_DOUBLE, 2, 4, false };
  static program::data_type gl_double_mat_3x2 = { 6, GL_DOUBLE, 3, 2, false };
  static program::data_type gl_double_mat_3 = { 9, GL_DOUBLE, 3, 3, false };
  static program::data_type gl_double_mat_3x4 = { 12, GL_DOUBLE, 3, 4, false };
  static program::data_type gl_double_mat_4x2 = { 8, GL_DOUBLE, 4, 2, false };
  static program::data_type gl_double_mat_4x3 = { 12, GL_DOUBLE, 4, 3, false };
  static program::data_type gl_double_mat_4 = { 16, GL_DOUBLE, 4, 4, false };

  program::data_type* attribute_lookup_table(int32_t gl_type)
  {
    switch(gl_type)
    {
      case GL_INT: return &gl_int;
      case GL_INT_VEC2: return &gl_int_vec2;
      case GL_INT_VEC3: return &gl_int_vec3;
      case GL_INT_VEC4: return &gl_int_vec4;
      case GL_UNSIGNED_INT: return &gl_unsigned_int;
      case GL_UNSIGNED_INT_VEC2: return &gl_unsigned_int_vec2;
      case GL_UNSIGNED_INT_VEC3: return &gl_unsigned_int_vec3;
      case GL_UNSIGNED_INT_VEC4: return &gl_unsigned_int_vec4;
      case GL_FLOAT: return &gl_float;
      case GL_FLOAT_VEC2: return &gl_float_vec2;
      case GL_FLOAT_VEC3: return &gl_float_vec3;
      case GL_FLOAT_VEC4: return &gl_float_vec4;
      case GL_DOUBLE: return &gl_double;
      case GL_DOUBLE_VEC2: return &gl_double_vec2;
      case GL_DOUBLE_VEC3: return &gl_double_vec3;
      case GL_DOUBLE_VEC4: return &gl_double_vec4;
      case GL_FLOAT_MAT2: return &gl_float_mat_2;
      case GL_FLOAT_MAT2x3: return &gl_float_mat_2x3;
      case GL_FLOAT_MAT2x4: return &gl_float_mat_2x4;
      case GL_FLOAT_MAT3x2: return &gl_float_mat_3x2;
      case GL_FLOAT_MAT3: return &gl_float_mat_3;
      case GL_FLOAT_MAT3x4: return &gl_float_mat_3x4;
      case GL_FLOAT_MAT4x2: return &gl_float_mat_4x2;
      case GL_FLOAT_MAT4x3: return &gl_float_mat_4x3;
      case GL_FLOAT_MAT4: return &gl_float_mat_4;
      case GL_DOUBLE_MAT2: return &gl_double_mat_2;
      case GL_DOUBLE_MAT2x3: return &gl_double_mat_2x3;
      case GL_DOUBLE_MAT2x4: return &gl_double_mat_2x4;
      case GL_DOUBLE_MAT3x2: return &gl_double_mat_3x2;
      case GL_DOUBLE_MAT3: return &gl_double_mat_3;
      case GL_DOUBLE_MAT3x4: return &gl_double_mat_3x4;
      case GL_DOUBLE_MAT4x2: return &gl_double_mat_4x2;
      case GL_DOUBLE_MAT4x3: return &gl_double_mat_4x3;
      case GL_DOUBLE_MAT4: return &gl_double_mat_4;
      default: MGL_CORE_ASSERT(false, "invalid gl type"); return nullptr;
    };
  }

  program::program(const context_ref& ctx,
                   const shaders& shaders,
                   const shaders_outputs& outputs,
                   const fragment_outputs& fragment_outputs,
                   bool interleaved,
                   const std::string& filename)
      : gl_object(ctx)
      , m_filename(filename)
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
    MGL_CORE_ASSERT(glo, "[Program] Cannot create program.");

    int32_t shader_objs[] = { 0, 0, 0, 0, 0 };

    for(int32_t i = 0; i < shader::type::GENERIC_PROGRAM; ++i)
    {
      if(shaders.sources[i] == "")
      {
        continue;
      }

      const char* source_str = shaders.sources[i].c_str();

      int32_t shader_glo = glCreateShader(SHADER_TYPE[i]);
      MGL_CORE_ASSERT(shader_glo, "[Program] Cannot create shader object.");

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
          MGL_CORE_ERROR("[Program] [{0}] GLSL compilation failed '{1}': {2}",
                         m_filename.c_str(),
                         SHADER_NAME[i],
                         log);
        }
        else
        {
          MGL_CORE_ERROR("[Program] GLSL compilation failed '{0}': {1}", SHADER_NAME[i], log);
        }
        delete[] log;
        glDeleteShader(shader_glo);
        glDeleteProgram(glo);
        MGL_CORE_ASSERT(false, "[Program] GLSL Compiler failed.");
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
      int32_t log_len = 0;
      glGetProgramiv(glo, GL_INFO_LOG_LENGTH, &log_len);

      char* log = new char[log_len];
      glGetProgramInfoLog(glo, log_len, &log_len, log);
      MGL_CORE_ERROR("[Program] GLSL link failed: {0}", log);
      delete[] log;
      glDeleteProgram(glo);
      MGL_CORE_ASSERT(false, "[Program] GLSL Linker failed.");
      return;
    }

    gl_object::set_glo(glo);

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
    glGetProgramiv(gl_object::glo(), GL_ACTIVE_ATTRIBUTES, &num_attributes);

    for(int32_t i = 0; i < num_attributes; ++i)
    {
      GLenum type = 0;
      GLint array_length = 0;
      GLint name_len = 0;
      char name[256];

      glGetActiveAttrib(gl_object::glo(), i, 256, &name_len, &array_length, &type, name);
      int32_t location = glGetAttribLocation(gl_object::glo(), name);

      internal::clean_glsl_name(name, name_len);

      program::attribute attr = {
        name, attribute_lookup_table(type), location, (size_t)array_length
      };
      m_attributes_map.insert({ name, attr });
    }

    int32_t num_varyings = 0;
    glGetProgramiv(gl_object::glo(), GL_TRANSFORM_FEEDBACK_VARYINGS, &num_varyings);

    for(int32_t i = 0; i < num_varyings; ++i)
    {
      int32_t type = 0;
      int32_t array_length = 0;
      int32_t dimension = 0;
      int32_t name_len = 0;
      char name[256];

      glGetTransformFeedbackVarying(
          gl_object::glo(), i, 256, &name_len, &array_length, (GLenum*)&type, name);

      program::varying varying = { name, i, (size_t)array_length, dimension };
      m_varyings_map.insert({ name, varying });
    }

    int32_t num_uniforms = 0;
    glGetProgramiv(gl_object::glo(), GL_ACTIVE_UNIFORMS, &num_uniforms);

    for(int32_t i = 0; i < num_uniforms; ++i)
    {
      int32_t type = 0;
      int32_t size = 0;
      int32_t name_len = 0;
      char name[256];

      glGetActiveUniform(gl_object::glo(), i, 256, &name_len, &size, (GLenum*)&type, name);
      int32_t location = glGetUniformLocation(gl_object::glo(), name);

      internal::clean_glsl_name(name, name_len);

      if(location < 0)
      {
        continue;
      }

      m_uniforms_map.insert(
          { name,
            mgl::create_ref<mgl::opengl::uniform>(name, type, gl_object::glo(), location, size) });
    }

    int32_t num_uniform_blocks = 0;
    glGetProgramiv(gl_object::glo(), GL_ACTIVE_UNIFORM_BLOCKS, &num_uniform_blocks);

    for(int32_t i = 0; i < num_uniform_blocks; ++i)
    {
      int32_t size = 0;
      int32_t name_len = 0;
      char name[256];

      glGetActiveUniformBlockName(gl_object::glo(), i, 256, &name_len, name);
      int32_t index = glGetUniformBlockIndex(gl_object::glo(), name);
      glGetActiveUniformBlockiv(gl_object::glo(), index, GL_UNIFORM_BLOCK_DATA_SIZE, &size);

      internal::clean_glsl_name(name, name_len);

      m_uniform_blocks_map.insert(
          { name,
            mgl::create_ref<mgl::opengl::uniform_block>(name, gl_object::glo(), index, size) });
    }

    if(gl_object::ctx()->version() >= 400)
    {
      for(int32_t st = 0; st < shader::type::GENERIC_PROGRAM; ++st)
      {
        int32_t num_subroutines = 0;
        auto type = subroutine::type(SHADER_TYPE[st]);

        glGetProgramStageiv(glo, type, GL_ACTIVE_SUBROUTINES, &num_subroutines);

        int32_t num_subroutine_uniforms = 0;
        glGetProgramStageiv(glo, type, GL_ACTIVE_SUBROUTINE_UNIFORMS, &num_subroutine_uniforms);

        for(int32_t i = 0; i < num_subroutines; ++i)
        {
          int32_t name_len = 0;
          char name[256];

          glGetActiveSubroutineName(glo, type, i, 256, &name_len, name);
          int32_t index = glGetSubroutineIndex(glo, type, name);

          subroutine sub = { name, index, type };
          m_subroutines_map.insert({ name, sub });
        }
      }
    }
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[Program] Error on creating program.");
  }

  void program::release()
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Program] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Program] Resource context not current.");
    glDeleteProgram(gl_object::glo());
    gl_object::set_glo(0);
  }

  void program::bind()
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Program] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Program] Resource context not current.");
    glUseProgram(gl_object::glo());
  }

  void program::unbind()
  {
    MGL_CORE_ASSERT(!gl_object::released() != 0,
                    "[Program] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Program] Resource context not current.");
    glUseProgram(GL_ZERO);
  }

} // namespace  mgl::opengl
