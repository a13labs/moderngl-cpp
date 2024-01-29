#include "mgl_graphics/managers/buffer.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_core/string.hpp"

namespace mgl::graphics
{

  static mgl::scope<buffer_manager> s_instance = nullptr;

  buffer_manager::buffer_manager()
  {
    MGL_CORE_ASSERT(s_instance == nullptr, "Only one buffer manager can exist at a time");
    s_instance = mgl::scope<buffer_manager>(this);
  }

  buffer_manager::~buffer_manager()
  {
    s_instance.release();
  }

  bool buffer_manager::on_add(const buffer_ref& buffer)
  {
    buffer->allocate();
    return true;
  }

  void buffer_manager::on_remove(const buffer_ref& buffer)
  {
    buffer->free();
  }

  buffer_manager& buffer_manager::instance()
  {
    MGL_CORE_ASSERT(s_instance != nullptr, "Buffer manager does not exists");
    return *s_instance;
  }

} // namespace mgl::graphics
