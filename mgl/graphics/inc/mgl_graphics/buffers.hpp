#pragma once

#include "mgl_core/containers.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"
#include "mgl_window/api.hpp"

namespace mgl::graphics
{
  class vertex_buffer
  {
public:
    vertex_buffer(const mgl::buffer<float>& data, const std::string& layout, bool dynamic = false);

    ~vertex_buffer();

    const std::string& layout() const { return m_layout; }

    const mgl::window::api::buffer_ref& buffer() const { return m_buffer; }

    void upload(const mgl::buffer<float>& data);

    void upload(const void* src, size_t size);

    void orphan(size_t size);

    bool is_dynamic() const { return m_dynamic; }

    void release();

protected:
    mgl::window::api::buffer_ref m_buffer;
    std::string m_layout;
    bool m_dynamic;
  };

  using vertex_buffer_ref = mgl::ref<vertex_buffer>;

  class index_buffer
  {
public:
    index_buffer(const mgl::buffer<float>& data = {}, bool dynamic = false);
    ~index_buffer();

    const mgl::window::api::buffer_ref& buffer() const { return m_buffer; }

    void upload(const mgl::buffer<float>& data);

    void upload(const void* src, size_t size);

    void orphan(size_t size);

    bool is_dynamic() const { return m_dynamic; }

    void release();

protected:
    mgl::window::api::buffer_ref m_buffer;
    bool m_dynamic;
  };

  using index_buffer_ref = mgl::ref<index_buffer>;

} // namespace mgl::graphics