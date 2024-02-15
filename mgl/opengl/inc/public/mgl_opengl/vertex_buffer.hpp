#pragma once

#include "buffer.hpp"
#include "buffer_layout.hpp"

#include "mgl_core/containers.hpp"
#include "mgl_core/string.hpp"

namespace mgl::opengl
{
  class vertex_buffer
  {

public:
    vertex_buffer(const buffer_ref& buffer,
                  const buffer_layout& layout,
                  const mgl::string_list& attributes)
        : m_buffer(buffer)
        , m_layout(layout)
        , m_attributes(attributes)
    {
      MGL_CORE_ASSERT(layout.size() > 0, "Buffer layout is empty");
      MGL_CORE_ASSERT(layout.size() == attributes.size(), "Layout and attributes size mismatch");
      if(m_buffer)
      {
        m_vertex_count = m_buffer->size() / m_layout.size();
      }
    }

    vertex_buffer(buffer_ref buffer, const std::string& layout, mgl::string_list attributes)
        : vertex_buffer(buffer, buffer_layout(layout), attributes)
    { }

    const buffer_ref& buffer() const { return m_buffer; }

    const buffer_layout& layout() const { return m_layout; }

    const mgl::string_list& attributes() const { return m_attributes; }

    int32_t vertex_count() const { return m_vertex_count; }

    void update(const buffer_ref& buffer,
                const buffer_layout& layout,
                const mgl::string_list& attributes)
    {
      MGL_CORE_ASSERT(buffer, "Buffer is null");
      MGL_CORE_ASSERT(layout.size() > 0, "Buffer layout is empty");
      MGL_CORE_ASSERT(layout.size() == attributes.size(), "Layout and attributes size mismatch");
      m_buffer = buffer;
      m_layout = layout;
      m_attributes = attributes;
      m_vertex_count = m_buffer->size() / m_layout.size();
    }

    void update(const buffer_ref& buffer, const std::string& layout)
    {
      update(buffer, buffer_layout(layout), m_attributes);
    }

    void update(const buffer_ref& buffer) { update(buffer, m_layout, m_attributes); }

    void release()
    {
      m_buffer = nullptr;
      m_layout = "";
      m_vertex_count = 0;
    }

    bool is_valid() const { return m_buffer && m_layout.size() > 0; }

    bool operator==(const vertex_buffer& other) const
    {
      return m_buffer == other.m_buffer && m_layout == other.m_layout;
    }

    bool operator!=(const vertex_buffer& other) const { return !(*this == other); }

    const buffer_layout::element& get_element(int index) const
    {
      return m_layout.elements()[index];
    }

    const std::string& get_attribute(int index) const { return m_attributes[index]; }

    int stride() const { return m_layout.stride(); }

private:
    buffer_ref m_buffer;
    buffer_layout m_layout;
    mgl::string_list m_attributes;
    int32_t m_vertex_count = 0;
  };

  using vertex_buffer_list = mgl::list<vertex_buffer>;
} // namespace mgl::opengl