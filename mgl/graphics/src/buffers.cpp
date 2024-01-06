#include "mgl_graphics/buffers.hpp"

#include "mgl_window/window.hpp"

namespace mgl::graphics
{
  vertex_buffer::vertex_buffer(const mgl::buffer<float>& data,
                               const std::string& layout,
                               bool dynamic)
      : m_layout(layout)
      , m_dynamic(dynamic)
  {
    m_buffer = mgl::window::current_context()->buffer(data, dynamic);
  }

  void vertex_buffer::upload(const mgl::buffer<float>& data)
  {
    m_buffer->write(data);
  }

  void vertex_buffer::upload(const void* src, size_t size)
  {
    m_buffer->write(src, size);
  }

  void vertex_buffer::orphan(size_t size)
  {
    m_buffer->orphan(size);
  }

  vertex_buffer::~vertex_buffer()
  {
    m_buffer->release();
  }

  void vertex_buffer::release()
  {
    m_buffer->release();
  }

  index_buffer::index_buffer(const mgl::buffer<float>& data, bool dynamic)
      : m_dynamic(dynamic)
  {
    m_buffer = mgl::window::current_context()->buffer(data, false);
  }

  void index_buffer::upload(const mgl::buffer<float>& data)
  {
    m_buffer->write(data);
  }

  void index_buffer::upload(const void* src, size_t size)
  {
    m_buffer->write(src, size);
  }

  void index_buffer::orphan(size_t size)
  {
    m_buffer->orphan(size);
  }

  index_buffer::~index_buffer()
  {
    m_buffer->release();
  }

  void index_buffer::release()
  {
    m_buffer->release();
  }

} // namespace mgl::graphics
