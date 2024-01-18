#include "mgl_graphics/buffer.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_window/window.hpp"

namespace mgl::graphics
{
  void buffer::upload(const void* data, size_t size)
  {
    MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
    m_buffer->write(data, size);
  }

  void buffer::orphan(size_t size)
  {
    MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
    m_buffer->orphan(size);
  }

  void buffer::allocate()
  {
    MGL_CORE_ASSERT(!m_buffer, "Buffer is already allocated");
    m_buffer = mgl::window::current_context()->buffer(is_dynamic());
    m_buffer->orphan(m_size);
  }

  void buffer::free()
  {
    MGL_CORE_ASSERT(m_buffer, "Buffer is not allocated");
    m_buffer->release();
    m_buffer = nullptr;
  }

} // namespace mgl::graphics
