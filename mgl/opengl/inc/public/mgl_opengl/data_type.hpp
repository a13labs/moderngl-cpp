#pragma once

#include <string>

namespace mgl::opengl
{

  struct data_type
  {
    int32_t* base_format;
    int32_t* internal_format;
    int32_t gl_type;
    int32_t size;
    bool float_type;
  };

  data_type* from_dtype(const char* dtype, size_t size);
  inline data_type* from_dtype(const std::string& dtype)
  {
    return from_dtype(dtype.c_str(), dtype.length());
  }

} // namespace  mgl::opengl
