
#include "mgl_game/integrations/imgui.hpp"
#include "mgl_game/window.hpp"
#include "mgl_opengl/context.hpp"

#include "imgui/imgui.h"

class game_window : public mgl::game::window
{

  public:
  virtual void on_draw(float time, float frame_time) override;
  virtual void on_load() override;
  virtual void on_unload() override;

  virtual void on_event(mgl::game::event& event) override;

  private:
  mgl::opengl::program_ref m_program;
  mgl::opengl::buffer_ref m_vbo;
  mgl::opengl::buffer_ref m_ibo;
  mgl::opengl::vertex_array_ref m_vao;
};

static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void game_window::on_draw(float time, float frame_time)
{
  ImGuiIO& io = ImGui::GetIO();

  static bool show_demo_window = true;
  static bool show_another_window = false;
  static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  ImGui::NewFrame();
  {
    static float f = 0.0f;
    static int counter = 0;
    ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

    ImGui::Text(
        "This is some useful text."); // Display some text (you can use a format strings too)
    ImGui::Checkbox("Demo Window",
                    &show_demo_window); // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    if(ImGui::Button(
           "Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
      counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text(
        "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
  }

  const auto ctx = context();
  ctx->clear(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

  m_vao->render();
  mgl::game::imgui::render();
}

void game_window::on_load()
{
  mgl::game::imgui::initialize();
  set_title("Index Buffer");

  const auto ctx = context();
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

  mgl::buffer<float> vertices = {
    0.0,  0.0, //

    -0.6, -0.8, //
    0.6,  -0.8, //

    0.6,  0.8, //
    -0.6, 0.8, //
  };

  mgl::buffer<uint32_t> indices = { 0, 1, 2, 0, 3, 4 };

  m_vbo = ctx->buffer(vertices);
  m_ibo = ctx->buffer(indices);
  mgl::opengl::vertex_buffer_list m_content = { { m_vbo, "2f", { "in_vert" } } };

  m_vao = ctx->vertex_array(m_program, m_content, m_ibo);
}

void game_window::on_event(mgl::game::event& event)
{
  mgl::game::window::on_event(event);
  mgl::game::imgui::on_event(event);
}

void game_window::on_unload()
{
  mgl::game::imgui::shutdown();
  m_ibo->release();
  m_vao->release();
  m_vbo->release();
  m_program->release();
}

int main(int argc, char* argv[])
{
  game_window app;
  app.run();

  return 0;
}