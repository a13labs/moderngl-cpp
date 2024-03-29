#include <cmath>

#include "mgl_core/math.hpp"
#include "mgl_opengl/context.hpp"
#include "mgl_platform/api/opengl/api.hpp"
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
  mgl::opengl::vertex_array_ref m_vao;
  mgl::opengl::uniform_ref m_scale, m_rotation;
  float m_time = 0;
};

void example::on_update(float time, float frame_time)
{
  const auto ctx = mgl::platform::api::backends::ogl_api::current_context();

  float sin_scale = static_cast<float>(sin(mgl::math::deg2rad(time * 60)));

  ctx->clear(1.0, 1.0, 1.0);
  ctx->enable(mgl::opengl::enable_flag::BLEND);
  m_vao->render();

  m_rotation->set_value(time);
  m_scale->set_value(glm::vec2(sin_scale * 0.75, 0.75));
}

bool example::on_load()
{
  set_title("Uniforms and Attributes Example");

  const auto ctx = mgl::platform::api::backends::ogl_api::current_context();
  m_program = ctx->program({
      R"(
                #version 330
                in vec2 vert;

                uniform vec2 scale;
                uniform float rotation;

                void main() {

                    mat2 rot = mat2(
                        cos(rotation), sin(rotation),
                        -sin(rotation), cos(rotation)
                    );
                    gl_Position = vec4((rot * vert) * scale, 0.0, 1.0);
                }
              )",
      R"(
                #version 330

                out vec4 color;
                void main() {
                    color = vec4(0.3, 0.5, 1.0, 1.0);
                }
            )",

  });

  m_scale = m_program->uniform("scale");
  m_rotation = m_program->uniform("rotation");

  m_scale->set_value(glm::vec2(width() / height() * 0.75, 0.25));

  mgl::float32_buffer vertices = {
    1.0,  0.0, //
    -0.5, 0.86, //
    -0.5, -0.86 //
  };

  m_vbo = ctx->buffer(vertices);
  m_vao = ctx->vertex_array(m_program, { { m_vbo, "2f", { "vert" } } });

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