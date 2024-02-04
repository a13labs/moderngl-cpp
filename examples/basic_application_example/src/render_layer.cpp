#include "layers.hpp"
#include "shader.hpp"

#include "imgui/imgui.h"
#include "mgl_application/application.hpp"
#include "mgl_graphics/buffer.hpp"
#include "mgl_graphics/graphics.hpp"

static mgl::graphics::vertex_buffer_ref s_vbo = nullptr;

void render_layer::render_prepare(mgl::graphics::render_script& script)
{
  script.clear(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  script.enable_shader("custom_shader");
  script.draw(s_vbo);
  script.disable_shader();
  script.draw_text("This is a text size 8!", glm::vec2(0.0), glm::vec4(1.0, 0.0, 0.0, 1.0));
  script.draw_text(
      "This is a text size 16!", glm::vec2(0.0, 32.f), glm::vec4(1.0, 0.0, 0.0, 1.0), 16.f);
  script.draw_text(
      "This is a text size 32!", glm::vec2(0.0, 64.f), glm::vec4(1.0, 0.0, 0.0, 1.0), 32.f);
}

void render_layer::on_attach()
{
  mgl::float32_buffer vertices = {
    // x, y, red, green, blue
    0.0,  0.8,  1.0, 0.0, 0.0, //
    -0.6, -0.8, 0.0, 1.0, 0.0, //
    0.6,  -0.8, 0.0, 0.0, 1.0, //
  };

  s_vbo = mgl::create_ref<mgl::graphics::vertex_buffer>("2f 3f", vertices.size() * sizeof(float));
  s_vbo->allocate();
  s_vbo->upload(vertices);

  register_shader("custom_shader", mgl::create_ref<custom_shader>());
}

void render_layer::on_detach()
{
  s_vbo->free();
}
