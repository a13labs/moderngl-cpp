#include "mgl_graphics/render.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_graphics/commands/draw.hpp"
#include "mgl_graphics/commands/functions.hpp"
#include "mgl_graphics/commands/shader.hpp"
#include "mgl_graphics/commands/state.hpp"
#include "mgl_graphics/commands/texture.hpp"
namespace mgl::graphics
{

  void render::release()
  {
    m_buffer_manager.clear();
    m_shader_manager.clear();
    m_texture_manager.clear();
  }
} // namespace mgl::graphics