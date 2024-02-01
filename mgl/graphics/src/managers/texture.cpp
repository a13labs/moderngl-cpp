#include "mgl_graphics/managers/texture.hpp"
#include "mgl_core/debug.hpp"

namespace mgl::graphics
{

  static mgl::scope<texture_manager> s_instance = nullptr;

  texture_manager::texture_manager()
  {
    MGL_CORE_ASSERT(s_instance == nullptr, "Texture manager already exists");
    s_instance = mgl::scope<texture_manager>(this);
  }

  texture_manager::~texture_manager()
  {
    s_instance.release();
  }

  void texture_manager::on_add(const texture_ref& texture, const std::string& name)
  {
    texture->load();
  }

  void texture_manager::on_remove(const texture_ref& texture, const std::string& name)
  {
    texture->unload();
  }

  texture_manager& texture_manager::instance()
  {
    return *s_instance;
  }

} // namespace mgl::graphics