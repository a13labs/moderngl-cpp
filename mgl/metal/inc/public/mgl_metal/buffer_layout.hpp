#pragma once

#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"

namespace mgl::metal
{
  class context;
  using context_ref = mgl::ref<context>;

  class buffer_layout
  {
public:
    struct attribute
    {
      std::string name;
      int32_t location;
      int32_t size;
      int32_t type;
      bool normalized;
      int32_t stride;
      int32_t offset;
      int32_t divisor;
    };

    buffer_layout(const std::string& layout);
    ~buffer_layout() = default;

    const std::string& layout() const { return m_layout; }
    int32_t vertex_size() const { return m_vertex_size; }
    const std::vector<attribute>& attributes() const { return m_attributes; }

private:
    void parse_layout(const std::string& layout);

    std::string m_layout;
    int32_t m_vertex_size;
    std::vector<attribute> m_attributes;
  };

} // namespace mgl::metal
