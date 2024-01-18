#include "mgl_opengl/context.hpp"
#include "mgl_window/window.hpp"
#include <iostream>

class example : public mgl::window::window
{

  public:
  virtual void on_update(float time, float frame_time) override;
  virtual bool on_load() override;
  virtual void on_unload() override;

  private:
  mgl::opengl::program_ref m_program;
  mgl::opengl::buffer_ref m_vbo;
  mgl::opengl::vertex_array_ref m_vao;
};

void example::on_update(float time, float frame_time)
{
  const auto ctx = current_context();
  ctx->clear(1.0, 1.0, 1.0);
  m_vao->render();
}

bool example::on_load()
{
  set_title("Simple Color Triangle");

  const auto ctx = current_context();
  m_program = ctx->program({
      R"(
                #version 330

                in vec2 in_vert;
                in vec3 in_color;
                out vec3 v_color;    // Goes to the fragment shader

                void main() {
                    gl_Position = vec4(in_vert, 0.0, 1.0);
                    v_color = in_color;
                }
              )",
      R"(
                #version 330

                in vec3 v_color;
                out vec4 f_color;

                void main() {
                    // We're not interested in changing the alpha value
                    f_color = vec4(v_color, 1.0);
                }
            )",

  });

  mgl::float_buffer vertices = {
    // x, y, red, green, blue
    0.0,  0.8,  1.0, 0.0, 0.0, //
    -0.6, -0.8, 0.0, 1.0, 0.0, //
    0.6,  -0.8, 0.0, 0.0, 1.0, //
  };

  m_vbo = ctx->buffer(vertices);
  m_vao = ctx->vertex_array(m_program, { { m_vbo, "2f 3f", { "in_vert", "in_color" } } });

  return true;
}

void example::on_unload()
{
  m_vao->release();
  m_vbo->release();
  m_program->release();
}

int main(int argc, char* argv[])
{
  example app;
  app.run();

  return 0;
}