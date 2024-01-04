#include "layers.hpp"
#include "material.hpp"

#include "imgui/imgui.h"
#include "mgl_engine/application.hpp"
#include "mgl_engine/buffers.hpp"
#include "mgl_engine/render.hpp"

static mgl::engine::vertex_buffer_ref s_vbo = nullptr;
static mgl::engine::material_ref mat = nullptr;

void render_layer::render_prepare()
{
  auto& renderer = mgl::engine::current_render();
  renderer->clear(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  renderer->enable_material(mat);
  renderer->draw(s_vbo, nullptr, mgl::engine::render::draw_mode::TRIANGLES);
  renderer->disable_material();
}

void render_layer::on_attach()
{
  mgl::buffer<float> vertices = {
    // x, y, red, green, blue
    0.0,  0.8,  1.0, 0.0, 0.0, //
    -0.6, -0.8, 0.0, 1.0, 0.0, //
    0.6,  -0.8, 0.0, 0.0, 1.0, //
  };

  s_vbo = mgl::create_ref<mgl::engine::vertex_buffer>(vertices, "2f 3f");
  mat = mgl::create_ref<custom_material>();
}

void render_layer::on_detach()
{
  mat->release();
  s_vbo->release();
}
