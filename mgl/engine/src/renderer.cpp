#include "mgl_engine/renderer.hpp"
#include "mgl_engine/commands/common.hpp"
#include "mgl_engine/commands/state.hpp"
#include "mgl_engine/commands/texture.hpp"

namespace mgl::engine
{
  void renderer::flush()
  {
    auto ctx = context();

    ctx->enter();

    for(auto& command : m_render_queue)
    {
      command->execute();
    }

    ctx->exit();
  }

  void renderer::begin()
  {
    m_render_queue.clear();
  }

  void renderer::end()
  {
    flush();
  }

  void renderer::enable_state(state state)
  {
    submit(mgl::create_ref<mgl::engine::enable_state>(state));
  }

  void renderer::disable_state(state state)
  {
    submit(mgl::create_ref<mgl::engine::disable_state>(state));
  }

  void renderer::enable_texture(uint32_t slot, const mgl::window::texture_ref& t)
  {
    submit(mgl::create_ref<mgl::engine::enable_texture>(slot, t));
  }

  void renderer::disable_texture(uint32_t slot)
  {
    submit(mgl::create_ref<mgl::engine::disable_texture>(slot));
  }

  void renderer::clear(const glm::vec4& color)
  {
    submit(mgl::create_ref<mgl::engine::clear_command>(color));
  }

  void renderer::set_view(const glm::mat4& view)
  {
    submit(mgl::create_ref<mgl::engine::set_view_command>(view));
  }

  void renderer::set_projection(const glm::mat4& projection)
  {
    submit(mgl::create_ref<mgl::engine::set_projection_command>(projection));
  }

  void renderer::set_blend_func(blend_func src, blend_func dst) { }

  void renderer::set_color_mask(bool r, bool g, bool b, bool a) { }

  void renderer::set_depth_mask(bool mask) { }

  void renderer::set_depth_func(depth_func func) { }

  void renderer::set_stencil_mask(uint32_t mask) { }

  void renderer::set_stencil_func(stencil_func func, int32_t ref, uint32_t mask) { }

  void renderer::set_stencil_op(stencil_op sfail, stencil_op dpfail, stencil_op dppass) { }

  void renderer::set_cull_face(cull_face face) { }

  void renderer::set_polygon_offset(float factor, float units) { }

  void renderer::draw(const mgl::window::vertex_array_ref& vertex_array, uint32_t count) { }

  void renderer::enable_material(material_ref material) { }

  void renderer::disable_material() { }

} // namespace mgl::engine