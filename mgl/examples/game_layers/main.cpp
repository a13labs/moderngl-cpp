
#include "mgl_core/memory.hpp"

#include "mgl_engine/application.hpp"
#include "mgl_engine/layers/gui.hpp"
#include "mgl_engine/layers/render.hpp"

#include "imgui/imgui.h"

static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

class render_layer : public mgl::engine::layers::render_layer
{
  public:
  render_layer()
      : mgl::engine::layers::render_layer("Example Render Layer")
  { }

  virtual void on_attach() override;

  virtual void on_detach() override;

  virtual void render_prepare() override;
};

class gui_layer : public mgl::engine::layers::gui_layer
{
  public:
  gui_layer()
      : mgl::engine::layers::gui_layer("Example GUI Layer")
  { }
  virtual void draw_ui(float time, float frame_time) override;
};

class game : public mgl::engine::application
{
  public:
  game()
      : mgl::engine::application()
  {
    config().gui_layer = mgl::create_ref<gui_layer>();
    config().render_layer = mgl::create_ref<render_layer>();
  }
};

void gui_layer::draw_ui(float time, float frame_time)
{
  ImGuiIO& io = ImGui::GetIO();

  static bool show_demo_window = true;
  static bool show_another_window = false;

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
}

void render_layer::render_prepare()
{
  auto renderer = mgl::engine::current_renderer();
  renderer->clear(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
}

void render_layer::on_attach() { }

void render_layer::on_detach() { }

int main(int argc, char* argv[])
{
  game app;
  app.run();

  return 0;
}