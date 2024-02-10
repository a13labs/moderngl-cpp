#include "mgl_opengl/uniform.hpp"
#include "mgl_opengl/context.hpp"

#include "mgl_core/debug.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  static uniform::data_type gl_bool = { false, 1, 4 };
  static uniform::data_type gl_bool_vec2 = { false, 2, 8 };
  static uniform::data_type gl_bool_vec3 = { false, 3, 12 };
  static uniform::data_type gl_bool_vec4 = { false, 4, 16 };
  static uniform::data_type gl_int = { false, 1, 4 };
  static uniform::data_type gl_int_vec2 = { false, 2, 8 };
  static uniform::data_type gl_int_vec3 = { false, 3, 12 };
  static uniform::data_type gl_int_vec4 = { false, 4, 16 };
  static uniform::data_type gl_unsigned_int = { false, 1, 4 };
  static uniform::data_type gl_unsigned_int_vec2 = { false, 2, 8 };
  static uniform::data_type gl_unsigned_int_vec3 = { false, 3, 12 };
  static uniform::data_type gl_unsigned_int_vec4 = { false, 4, 16 };
  static uniform::data_type gl_float = { false, 1, 4 };
  static uniform::data_type gl_float_vec2 = { false, 2, 8 };
  static uniform::data_type gl_float_vec3 = { false, 3, 12 };
  static uniform::data_type gl_float_vec4 = { false, 4, 16 };
  static uniform::data_type gl_double = { false, 1, 8 };
  static uniform::data_type gl_double_vec2 = { false, 2, 16 };
  static uniform::data_type gl_double_vec3 = { false, 3, 24 };
  static uniform::data_type gl_double_vec4 = { false, 4, 32 };
  static uniform::data_type gl_sampler_1_d = { false, 1, 4 };
  static uniform::data_type gl_sampler_1D_array = { false, 1, 4 };
  static uniform::data_type gl_int_sampler_1D = { false, 1, 4 };
  static uniform::data_type gl_int_sampler_1D_array = { false, 1, 4 };
  static uniform::data_type gl_sampler_2D = { false, 1, 4 };
  static uniform::data_type gl_int_sampler_2D = { false, 1, 4 };
  static uniform::data_type gl_unsigned_int_sampler_2D = { false, 1, 4 };
  static uniform::data_type gl_sampler_2D_array = { false, 1, 4 };
  static uniform::data_type gl_int_sampler_2D_array = { false, 1, 4 };
  static uniform::data_type gl_unsigned_int_sampler_2D_array = { false, 1, 4 };
  static uniform::data_type gl_sampler_3D = { false, 1, 4 };
  static uniform::data_type gl_int_sampler_3D = { false, 1, 4 };
  static uniform::data_type gl_unsigned_int_sampler_3D = { false, 1, 4 };
  static uniform::data_type gl_sampler_2D_shadow = { false, 1, 4 };
  static uniform::data_type gl_sampler_2D_multisample = { false, 1, 4 };
  static uniform::data_type gl_int_sampler_2D_multisample = { false, 1, 4 };
  static uniform::data_type gl_unsigned_int_sampler_2D_multisample = { false, 1, 4 };
  static uniform::data_type glSampler_2D_multisample_array = { false, 1, 4 };
  static uniform::data_type gl_int_sampler_2D_multisample_array = { false, 1, 4 };
  static uniform::data_type gl_unsigned_int_sampler_2D_multisample_array = { false, 1, 4 };
  static uniform::data_type gl_sampler_cube = { false, 1, 4 };
  static uniform::data_type gl_int_sampler_cube = { false, 1, 4 };
  static uniform::data_type gl_unsigned_int_sampler_cube = { false, 1, 4 };
  static uniform::data_type gl_image_2D = { false, 1, 4 };
  static uniform::data_type gl_float_mat_2 = { true, 4, 16 };
  static uniform::data_type gl_float_mat_2x3 = { true, 6, 24 };
  static uniform::data_type gl_float_mat_2x4 = { true, 8, 32 };
  static uniform::data_type gl_float_mat_3x2 = { true, 6, 24 };
  static uniform::data_type gl_float_mat_3 = { true, 9, 36 };
  static uniform::data_type gl_float_mat_3x4 = { true, 12, 48 };
  static uniform::data_type gl_float_mat_4x2 = { true, 8, 32 };
  static uniform::data_type gl_float_mat_4x3 = { true, 12, 48 };
  static uniform::data_type gl_float_mat_4 = { true, 16, 64 };
  static uniform::data_type gl_double_mat_2 = { true, 4, 32 };
  static uniform::data_type gl_double_mat_2x3 = { true, 6, 48 };
  static uniform::data_type gl_double_mat_2x4 = { true, 8, 64 };
  static uniform::data_type gl_double_mat_3x2 = { true, 6, 48 };
  static uniform::data_type gl_double_mat_3 = { true, 9, 72 };
  static uniform::data_type gl_double_mat_3x4 = { true, 12, 96 };
  static uniform::data_type gl_double_mat_4x2 = { true, 8, 64 };
  static uniform::data_type gl_double_mat_4x3 = { true, 12, 96 };
  static uniform::data_type gl_double_mat_4 = { true, 16, 128 };

  uniform::data_type* uniform_lookup_table(int gl_type)
  {
    switch(gl_type)
    {
      case GL_BOOL: return &gl_bool;
      case GL_BOOL_VEC2: return &gl_bool_vec2;
      case GL_BOOL_VEC3: return &gl_bool_vec3;
      case GL_BOOL_VEC4: return &gl_bool_vec4;
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
      case GL_SAMPLER_1D: return &gl_sampler_1_d;
      case GL_SAMPLER_1D_ARRAY: return &gl_sampler_1D_array;
      case GL_INT_SAMPLER_1D: return &gl_int_sampler_1D;
      case GL_INT_SAMPLER_1D_ARRAY: return &gl_int_sampler_1D_array;
      case GL_SAMPLER_2D: return &gl_sampler_2D;
      case GL_INT_SAMPLER_2D: return &gl_int_sampler_2D;
      case GL_UNSIGNED_INT_SAMPLER_2D: return &gl_unsigned_int_sampler_2D;
      case GL_SAMPLER_2D_ARRAY: return &gl_sampler_2D_array;
      case GL_INT_SAMPLER_2D_ARRAY: return &gl_int_sampler_2D_array;
      case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY: return &gl_unsigned_int_sampler_2D_array;
      case GL_SAMPLER_3D: return &gl_sampler_3D;
      case GL_INT_SAMPLER_3D: return &gl_int_sampler_3D;
      case GL_UNSIGNED_INT_SAMPLER_3D: return &gl_unsigned_int_sampler_3D;
      case GL_SAMPLER_2D_SHADOW: return &gl_sampler_2D_shadow;
      case GL_SAMPLER_2D_MULTISAMPLE: return &gl_sampler_2D_multisample;
      case GL_INT_SAMPLER_2D_MULTISAMPLE: return &gl_int_sampler_2D_multisample;
      case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: return &gl_unsigned_int_sampler_2D_multisample;
      case GL_SAMPLER_2D_MULTISAMPLE_ARRAY: return &glSampler_2D_multisample_array;
      case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return &gl_int_sampler_2D_multisample_array;
      case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
        return &gl_unsigned_int_sampler_2D_multisample_array;
      case GL_SAMPLER_CUBE: return &gl_sampler_cube;
      case GL_INT_SAMPLER_CUBE: return &gl_int_sampler_cube;
      case GL_UNSIGNED_INT_SAMPLER_CUBE: return &gl_unsigned_int_sampler_cube;
      case GL_IMAGE_2D: return &gl_image_2D;
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
      default: MGL_CORE_ASSERT(false, "[Uniform] Invalid gl type."); return nullptr;
    };
  }

  uniform::uniform(const std::string& name, int gl_type, int program_obj, int location, size_t size)
  {
    m_name = name;
    m_gl_type = gl_type;
    m_program_obj = program_obj;
    m_location = location;
    m_size = size;
    m_data_type = uniform_lookup_table(gl_type);
    m_data = new uint8_t[m_data_type->element_size * size]();
  }

  uniform::~uniform()
  {
    delete[] m_data;
  }

  void uniform::set_value(void* data, size_t size)
  {
    MGL_CORE_ASSERT(size == (size_t)(m_size * m_data_type->element_size),
                    "[Uniform] Invalid data size.");

    char* ptr = (char*)data;

    glUseProgram(m_program_obj);

    switch(m_gl_type)
    {
      case GL_BOOL: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_BOOL_VEC2: {
        glUniform2iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_BOOL_VEC3: {
        glUniform3iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_BOOL_VEC4: {
        glUniform4iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT_VEC2: {
        glUniform2iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT_VEC3: {
        glUniform3iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT_VEC4: {
        glUniform4iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_UNSIGNED_INT: {
        glUniform1uiv(m_location, m_size, (unsigned*)ptr);
      }
      break;
      case GL_UNSIGNED_INT_VEC2: {
        glUniform2uiv(m_location, m_size, (unsigned*)ptr);
      }
      break;
      case GL_UNSIGNED_INT_VEC3: {
        glUniform3uiv(m_location, m_size, (unsigned*)ptr);
      }
      break;
      case GL_UNSIGNED_INT_VEC4: {
        glUniform4uiv(m_location, m_size, (unsigned*)ptr);
      }
      break;
      case GL_FLOAT: {
        glUniform1fv(m_location, m_size, (float*)ptr);
      }
      break;
      case GL_FLOAT_VEC2: {
        glUniform2fv(m_location, m_size, (float*)ptr);
      }
      break;
      case GL_FLOAT_VEC3: {
        glUniform3fv(m_location, m_size, (float*)ptr);
      }
      break;
      case GL_FLOAT_VEC4: {
        glUniform4fv(m_location, m_size, (float*)ptr);
      }
      break;
      case GL_DOUBLE: {
        glUniform1dv(m_location, m_size, (double*)ptr);
      }
      break;
      case GL_DOUBLE_VEC2: {
        glUniform2dv(m_location, m_size, (double*)ptr);
      }
      break;
      case GL_DOUBLE_VEC3: {
        glUniform3dv(m_location, m_size, (double*)ptr);
      }
      break;
      case GL_DOUBLE_VEC4: {
        glUniform4dv(m_location, m_size, (double*)ptr);
      }
      break;
      case GL_SAMPLER_1D: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_SAMPLER_1D_ARRAY: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT_SAMPLER_1D: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT_SAMPLER_1D_ARRAY: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_SAMPLER_2D: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT_SAMPLER_2D: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_UNSIGNED_INT_SAMPLER_2D: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_SAMPLER_2D_ARRAY: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT_SAMPLER_2D_ARRAY: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_SAMPLER_3D: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT_SAMPLER_3D: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_UNSIGNED_INT_SAMPLER_3D: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_SAMPLER_2D_SHADOW: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_SAMPLER_2D_MULTISAMPLE: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT_SAMPLER_2D_MULTISAMPLE: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_SAMPLER_2D_MULTISAMPLE_ARRAY: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_SAMPLER_CUBE: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT_SAMPLER_CUBE: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_UNSIGNED_INT_SAMPLER_CUBE: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_IMAGE_2D: {
        glUniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_FLOAT_MAT2: {
        glUniformMatrix2fv(m_location, m_size, false, (float*)ptr);
      }
      break;
      case GL_FLOAT_MAT2x3: {
        glUniformMatrix2x3fv(m_location, m_size, false, (float*)ptr);
      }
      break;
      case GL_FLOAT_MAT2x4: {
        glUniformMatrix2x4fv(m_location, m_size, false, (float*)ptr);
      }
      break;
      case GL_FLOAT_MAT3x2: {
        glUniformMatrix3x2fv(m_location, m_size, false, (float*)ptr);
      }
      break;
      case GL_FLOAT_MAT3: {
        glUniformMatrix3fv(m_location, m_size, false, (float*)ptr);
      }
      break;
      case GL_FLOAT_MAT3x4: {
        glUniformMatrix3x4fv(m_location, m_size, false, (float*)ptr);
      }
      break;
      case GL_FLOAT_MAT4x2: {
        glUniformMatrix4x2fv(m_location, m_size, false, (float*)ptr);
      }
      break;
      case GL_FLOAT_MAT4x3: {
        glUniformMatrix4x3fv(m_location, m_size, false, (float*)ptr);
      }
      break;
      case GL_FLOAT_MAT4: {
        glUniformMatrix4fv(m_location, m_size, false, (float*)ptr);
      }
      break;
      case GL_DOUBLE_MAT2: {
        glUniformMatrix2dv(m_location, m_size, false, (double*)ptr);
      }
      break;
      case GL_DOUBLE_MAT2x3: {
        glUniformMatrix2x3dv(m_location, m_size, false, (double*)ptr);
      }
      break;
      case GL_DOUBLE_MAT2x4: {
        glUniformMatrix2x4dv(m_location, m_size, false, (double*)ptr);
      }
      break;
      case GL_DOUBLE_MAT3x2: {
        glUniformMatrix3x2dv(m_location, m_size, false, (double*)ptr);
      }
      break;
      case GL_DOUBLE_MAT3: {
        glUniformMatrix3dv(m_location, m_size, false, (double*)ptr);
      }
      break;
      case GL_DOUBLE_MAT3x4: {
        glUniformMatrix3x4dv(m_location, m_size, false, (double*)ptr);
      }
      break;
      case GL_DOUBLE_MAT4x2: {
        glUniformMatrix4x2dv(m_location, m_size, false, (double*)ptr);
      }
      break;
      case GL_DOUBLE_MAT4x3: {
        glUniformMatrix4x3dv(m_location, m_size, false, (double*)ptr);
      }
      break;
      case GL_DOUBLE_MAT4: {
        glUniformMatrix4dv(m_location, m_size, false, (double*)ptr);
      }
      break;
      default: MGL_CORE_ASSERT(false, "[Uniform] Invalid gl type."); break;
    }
  }

  void uniform::get_value(void* data, size_t size)
  {

    MGL_CORE_ASSERT(size == (size_t)(m_size * m_data_type->element_size),
                    "[Uniform] Invalid data size.");

    char* ptr = (char*)data;
    glUseProgram(m_program_obj);

    for(int i = 0; i < m_size; ++i)
    {
      switch(m_gl_type)
      {
        case GL_BOOL: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_BOOL_VEC2: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_BOOL_VEC3: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_BOOL_VEC4: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_INT: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_INT_VEC2: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_INT_VEC3: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_INT_VEC4: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_UNSIGNED_INT: {
          glGetUniformuiv(
              m_program_obj, m_location + i, (unsigned*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_UNSIGNED_INT_VEC2: {
          glGetUniformuiv(
              m_program_obj, m_location + i, (unsigned*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_UNSIGNED_INT_VEC3: {
          glGetUniformuiv(
              m_program_obj, m_location + i, (unsigned*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_UNSIGNED_INT_VEC4: {
          glGetUniformuiv(
              m_program_obj, m_location + i, (unsigned*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT: {
          glGetUniformfv(
              m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_VEC2: {
          glGetUniformfv(
              m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_VEC3: {
          glGetUniformfv(
              m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_VEC4: {
          glGetUniformfv(
              m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE: {
          glGetUniformdv(
              m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_VEC2: {
          glGetUniformdv(
              m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_VEC3: {
          glGetUniformdv(
              m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_VEC4: {
          glGetUniformdv(
              m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_SAMPLER_1D: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_SAMPLER_1D_ARRAY: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_INT_SAMPLER_1D: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_INT_SAMPLER_1D_ARRAY: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_SAMPLER_2D: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_INT_SAMPLER_2D: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_UNSIGNED_INT_SAMPLER_2D: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_SAMPLER_2D_ARRAY: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_INT_SAMPLER_2D_ARRAY: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_SAMPLER_3D: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_INT_SAMPLER_3D: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_UNSIGNED_INT_SAMPLER_3D: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_SAMPLER_2D_SHADOW: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_SAMPLER_2D_MULTISAMPLE: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_INT_SAMPLER_2D_MULTISAMPLE: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_SAMPLER_2D_MULTISAMPLE_ARRAY: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_SAMPLER_CUBE: {
          {
            glGetUniformiv(
                m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
          }
          break;
        }
        case GL_INT_SAMPLER_CUBE: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_UNSIGNED_INT_SAMPLER_CUBE: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_IMAGE_2D: {
          glGetUniformiv(
              m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_MAT2: {
          glGetUniformfv(
              m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_MAT2x3: {
          glGetUniformfv(
              m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_MAT2x4: {
          glGetUniformfv(
              m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_MAT3x2: {
          glGetUniformfv(
              m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_MAT3: {
          glGetUniformfv(
              m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_MAT3x4: {
          glGetUniformfv(
              m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_MAT4x2: {
          glGetUniformfv(
              m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_MAT4x3: {
          glGetUniformfv(
              m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_MAT4: {
          glGetUniformfv(
              m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_MAT2: {
          glGetUniformdv(
              m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_MAT2x3: {
          glGetUniformdv(
              m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_MAT2x4: {
          glGetUniformdv(
              m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_MAT3x2: {
          glGetUniformdv(
              m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_MAT3: {
          glGetUniformdv(
              m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_MAT3x4: {
          glGetUniformdv(
              m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_MAT4x2: {
          glGetUniformdv(
              m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_MAT4x3: {
          glGetUniformdv(
              m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_MAT4: {
          glGetUniformdv(
              m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        default: MGL_CORE_ASSERT(false, "[Uniform] Invalid gl type."); break;
      }
    }
  }
} // namespace  mgl::opengl
