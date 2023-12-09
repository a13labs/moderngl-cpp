#include "mgl_window/window.hpp"

#include "mgl_opengl/buffer.hpp"
#include "mgl_opengl/context.hpp"
#include "mgl_opengl/glsl_source.hpp"
#include "mgl_opengl/program.hpp"
#include "mgl_opengl/uniform.hpp"
#include "mgl_opengl/vertex_array.hpp"

class example_window : public mgl::window::window
{

  public:
  virtual void on_draw(float time, float frame_time) override;
  virtual void on_load() override;
  virtual void on_unload() override;

  private:
  mgl::opengl::program_ref m_program;
  mgl::opengl::buffer_ref m_vbo;
  mgl::opengl::vertex_array_ref m_vao;
  mgl::opengl::uniform_ref m_scale, m_rotation;
  float m_time = 0;
};

void example_window::on_draw(float time, float frame_time)
{
  const auto ctx = context();
  ctx->clear(1.0, 1.0, 1.0);
  ctx->enable(mgl::opengl::enable_flag::BLEND);
  m_rotation->set_value(time);
  m_vao->render(10);
}

void example_window::on_load()
{
  set_title("Alpha Blending");

  const auto ctx = context();
  m_program = ctx->program({
      R"(
                #version 330

                in vec2 vert;

                in vec4 vert_color;
                out vec4 frag_color;

                uniform vec2 scale;
                uniform float rotation;

                void main() {
                    frag_color = vert_color;
                    float r = rotation * (0.5 + gl_InstanceID * 0.05);
                    mat2 rot = mat2(cos(r), sin(r), -sin(r), cos(r));
                    gl_Position = vec4((rot * vert)*scale, 0.0, 1.0);
                    // gl_Position = vec4(vert, 0.0, 1.0);
                }
              )",
      R"(
                #version 330
                in vec4 frag_color;
                out vec4 color;
                void main() {
                    color = vec4(frag_color);
                }
            )",

  });

  m_scale = m_program->uniform("scale");
  m_rotation = m_program->uniform("rotation");

  m_scale->set_value({ 0.5, aspect_ratio() * 0.5 });

  mgl::core::mem_buffer<float> vertices = {
    1.0,  0.0, //
    1.0,  0.0,   0.0, 0.5, //

    -0.5, 0.86, //
    0.0,  1.0,   0.0, 0.5, //

    -0.5, -0.86, //
    0.0,  0.0,   1.0, 0.5, //
  };

  m_vbo = ctx->buffer(vertices);
  m_vao = ctx->vertex_array(m_program, { { m_vbo, "2f 4f", { "vert", "vert_color" } } });
}

void example_window::on_unload()
{
  m_vao->release();
  m_vbo->release();
  m_program->release();
}

int main(int argc, char* argv[])
{
  example_window app;
  app.run();

  return 0;
}