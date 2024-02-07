#pragma once

#include "mgl_core/containers.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"
namespace mgl::opengl
{
  class varying
  {
public:
    varying(const std::string& name, int32_t number, size_t array_length, int32_t m_dimension);

    ~varying() = default;

    const std::string& name() const { return m_name; }

    int32_t number() const { return m_number; }

    size_t array_length() const { return m_array_length; }

    int32_t dimension() const { return m_dimension; }

private:
    std::string m_name;
    int32_t m_number;
    size_t m_array_length;
    int32_t m_dimension;
  };

  using varying_ref = mgl::ref<varying>;
  using varyings_dict = mgl::dict<std::string, varying_ref>;
} // namespace  mgl::opengl
