
#include "mgl_opengl/context.hpp"
#include "mgl_platform/api/backends/opengl/api.hpp"
#include "mgl_platform/window.hpp"

class example : public mgl::platform::window
{

  public:
  virtual void on_update(float time, float frame_time) override;
  virtual bool on_load() override;
  virtual void on_unload() override;

  private:
  mgl::opengl::program_ref m_program;
  mgl::opengl::buffer_ref m_vbo;
  mgl::opengl::buffer_ref m_ibo;
  mgl::opengl::vertex_array_ref m_vao;
};

void example::on_update(float time, float frame_time)
{
  const auto ctx = mgl::platform::api::backends::opengl_api::current_context();
  ctx->clear(1.0, 1.0, 1.0);
  m_vao->render();
}

bool example::on_load()
{
  set_title("Index Buffer");

  const auto ctx = mgl::platform::api::backends::opengl_api::current_context();
  m_program = ctx->program({
      R"(
                #version 330

                in vec2 in_vert;

                void main() {
                  gl_Position = vec4(in_vert, 0.0, 1.0);
                }
              )",
      R"(
                #version 330

                out vec4 f_color;

                void main() {
                  f_color = vec4(0.3, 0.5, 1.0, 1.0);
                }
            )",

  });

  mgl::float32_buffer vertices = {
    0.0,  0.0, //

    -0.6, -0.8, //
    0.6,  -0.8, //

    0.6,  0.8, //
    -0.6, 0.8, //
  };

  mgl::uint32_buffer indices = { 0, 1, 2, 0, 3, 4 };

  m_vbo = ctx->buffer(vertices);
  m_ibo = ctx->buffer(indices);
  mgl::opengl::vertex_buffer_list m_content = { { m_vbo, "2f", { "in_vert" } } };

  m_vao = ctx->vertex_array(m_program, m_content, m_ibo);

  return true;
}

void example::on_unload()
{
  m_ibo->release();
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