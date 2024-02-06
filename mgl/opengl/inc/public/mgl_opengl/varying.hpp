#pragma once

#include "mgl_core/containers.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"
namespace mgl::opengl
{
  class varying
  {
public:
    varying(const std::string& name, int number, size_t array_length, int m_dimension);
    ~varying() = default;

private:
    std::string m_name;
    int m_number;
    size_t m_array_length;
    int m_dimension;
  };

  using varying_ref = mgl::ref<varying>;
  using varyings_dict = mgl::dict<std::string, varying_ref>;
} // namespace  mgl::opengl
