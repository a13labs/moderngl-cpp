#include "mgl_graphics/managers/shader.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_core/string.hpp"

namespace mgl::graphics
{

  static shader_manager* s_instance = nullptr;

  shader_manager::shader_manager()
  {
    MGL_CORE_ASSERT(s_instance == nullptr, "Only one shader manager can exist at a time");
    s_instance = this;
  }

  shader_manager::~shader_manager()
  {
    s_instance = nullptr;
  }

  void shader_manager::on_add(const shader_ref& shader)
  {
    shader->load();
  }

  void shader_manager::on_remove(const shader_ref& shader)
  {
    shader->unload();
  }

} // namespace mgl::graphics