#include "mgl_graphics/managers/shader.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_core/string.hpp"

namespace mgl::graphics
{

  static mgl::scope<shader_manager> s_instance = nullptr;

  shader_manager::shader_manager()
  {
    MGL_CORE_ASSERT(s_instance == nullptr, "Only one shader manager can exist at a time");
    s_instance = mgl::scope<shader_manager>(this);
  }

  shader_manager::~shader_manager()
  {
    s_instance.release();
  }

  void shader_manager::on_add(const shader_ref& shader)
  {
    shader->load();
  }

  void shader_manager::on_remove(const shader_ref& shader)
  {
    shader->unload();
  }

  shader_manager& shader_manager::instance()
  {
    MGL_CORE_ASSERT(s_instance != nullptr, "Shader manager does not exists");
    return *s_instance;
  }

} // namespace mgl::graphics