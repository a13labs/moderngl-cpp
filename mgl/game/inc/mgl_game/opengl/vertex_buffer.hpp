#pragma once

#include "mgl_core/containers.hpp"
#include "mgl_core/string.hpp"
#include "mgl_opengl/vertex_buffer.hpp"

namespace mgl::game::opengl
{

  struct buffer_element
  {
    const std::string format_string;
    const int components;
    const int byte_per_component;
    const bool per_instance;

    size_t size();
  };

  using buffer_layout = mgl::list<buffer_element>;

  extern const buffer_element null_buffer_format;

  buffer_layout parse_layout(const std::string& str);
  const buffer_element& to_buffer_element(const std::string& str);

  class vertex_buffer
  {

public:
    vertex_buffer(mgl::opengl::buffer_ref buffer,
                  const std::string& format,
                  const mgl::string_list& attrs = {},
                  bool per_instance = false);
    ~vertex_buffer() = default;

    size_t size();
    const buffer_layout& layout() const;
    const mgl::string_list& attributes() const;
    bool has_attribute(const std::string& name);
    const mgl::opengl::buffer_ref& buffer() const;
    bool per_instance();
    int vertices();

    operator mgl::opengl::vertex_buffer();

private:
    mgl::opengl::buffer_ref m_buffer;
    std::string m_layout_str;
    buffer_layout m_layout;
    mgl::string_list m_attrs;
    bool m_per_instance;
    int m_vertices;
  };

  using vertex_buffers_list = mgl::list<vertex_buffer>;

  inline size_t buffer_element::size()
  {
    return components * byte_per_component;
  }

  inline const buffer_layout& vertex_buffer::layout() const
  {
    return m_layout;
  }

  inline const mgl::string_list& vertex_buffer::attributes() const
  {
    return m_attrs;
  }

  inline bool vertex_buffer::has_attribute(const std::string& name)
  {
    return mgl::in(name, m_attrs);
  }

  inline const mgl::opengl::buffer_ref& vertex_buffer::buffer() const
  {
    return m_buffer;
  }

  inline vertex_buffer::operator mgl::opengl::vertex_buffer()
  {
    return { m_buffer, m_layout_str, m_attrs };
  }

  inline bool vertex_buffer::per_instance()
  {
    return m_per_instance;
  }

  inline int vertex_buffer::vertices()
  {
    return m_vertices;
  }
} // namespace mgl::game::opengl
