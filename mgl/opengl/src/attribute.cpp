
/*
   Copyright 2022 Alexandre Pires (c.alexandre.pires@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include "mgl_opengl/attribute.hpp"
#include "mgl_opengl/context.hpp"

#include "mgl_core/debug.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  static attribute::data_type gl_int = { 1, GL_INT, 1, 1, false, "i" };
  static attribute::data_type gl_int_vec2 = { 2, GL_INT, 1, 2, false, "i" };
  static attribute::data_type gl_int_vec3 = { 3, GL_INT, 1, 3, false, "i" };
  static attribute::data_type gl_int_vec4 = { 4, GL_INT, 1, 4, false, "i" };
  static attribute::data_type gl_unsigned_int = { 1, GL_UNSIGNED_INT, 1, 1, false, "i" };
  static attribute::data_type gl_unsigned_int_vec2 = { 2, GL_UNSIGNED_INT, 1, 2, false, "i" };
  static attribute::data_type gl_unsigned_int_vec3 = { 3, GL_UNSIGNED_INT, 1, 3, false, "i" };
  static attribute::data_type gl_unsigned_int_vec4 = { 4, GL_UNSIGNED_INT, 1, 4, false, "i" };
  static attribute::data_type gl_float = { 1, GL_FLOAT, 1, 1, true, "f" };
  static attribute::data_type gl_float_vec2 = { 2, GL_FLOAT, 1, 2, true, "f" };
  static attribute::data_type gl_float_vec3 = { 3, GL_FLOAT, 1, 3, true, "f" };
  static attribute::data_type gl_float_vec4 = { 4, GL_FLOAT, 1, 4, true, "f" };
  static attribute::data_type gl_double = { 1, GL_DOUBLE, 1, 1, false, "d" };
  static attribute::data_type gl_double_vec2 = { 2, GL_DOUBLE, 1, 2, false, "d" };
  static attribute::data_type gl_double_vec3 = { 3, GL_DOUBLE, 1, 3, false, "d" };
  static attribute::data_type gl_double_vec4 = { 4, GL_DOUBLE, 1, 4, false, "d" };
  static attribute::data_type gl_float_mat_2 = { 4, GL_FLOAT, 2, 2, true, "f" };
  static attribute::data_type gl_float_mat_2x3 = { 6, GL_FLOAT, 2, 3, true, "f" };
  static attribute::data_type gl_float_mat_2x4 = { 8, GL_FLOAT, 2, 4, true, "f" };
  static attribute::data_type gl_float_mat_3x2 = { 6, GL_FLOAT, 3, 2, true, "f" };
  static attribute::data_type gl_float_mat_3 = { 9, GL_FLOAT, 3, 3, true, "f" };
  static attribute::data_type gl_float_mat_3x4 = { 12, GL_FLOAT, 3, 4, true, "f" };
  static attribute::data_type gl_float_mat_4x2 = { 8, GL_FLOAT, 4, 2, true, "f" };
  static attribute::data_type gl_float_mat_4x3 = { 12, GL_FLOAT, 4, 3, true, "f" };
  static attribute::data_type gl_float_mat_4 = { 16, GL_FLOAT, 4, 4, true, "f" };
  static attribute::data_type gl_double_mat_2 = { 4, GL_DOUBLE, 2, 2, false, "d" };
  static attribute::data_type gl_double_mat_2x3 = { 6, GL_DOUBLE, 2, 3, false, "d" };
  static attribute::data_type gl_double_mat_2x4 = { 8, GL_DOUBLE, 2, 4, false, "d" };
  static attribute::data_type gl_double_mat_3x2 = { 6, GL_DOUBLE, 3, 2, false, "d" };
  static attribute::data_type gl_double_mat_3 = { 9, GL_DOUBLE, 3, 3, false, "d" };
  static attribute::data_type gl_double_mat_3x4 = { 12, GL_DOUBLE, 3, 4, false, "d" };
  static attribute::data_type gl_double_mat_4x2 = { 8, GL_DOUBLE, 4, 2, false, "d" };
  static attribute::data_type gl_double_mat_4x3 = { 12, GL_DOUBLE, 4, 3, false, "d" };
  static attribute::data_type gl_double_mat_4 = { 16, GL_DOUBLE, 4, 4, false, "d" };

  attribute::data_type* attribute_lookup_table(int gl_type)
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

  attribute::attribute(
      const std::string& name, int gl_type, int program_obj, int location, size_t array_length)
  {
    m_name = name;
    m_gl_type = gl_type;
    m_data_type = attribute_lookup_table(gl_type);
    m_program_obj = program_obj;
    m_location = location;
    m_array_length = array_length;
  }

} // namespace  mgl::opengl
