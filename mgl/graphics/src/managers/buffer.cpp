#include "mgl_graphics/managers/buffer.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_core/string.hpp"

namespace mgl::graphics
{

  static buffer_manager* s_instance = nullptr;

  buffer_manager::buffer_manager()
  {
    MGL_CORE_ASSERT(s_instance == nullptr, "Only one buffer manager can exist at a time");
    s_instance = this;
  }

  buffer_manager::~buffer_manager()
  {
    s_instance = nullptr;
  }

  void buffer_manager::on_add(const buffer_ref& buffer)
  {
    buffer->allocate();
  }

  void buffer_manager::on_remove(const buffer_ref& buffer)
  {
    buffer->free();
  }

} // namespace mgl::graphics
