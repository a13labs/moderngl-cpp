
#include "mgl_core/memory.hpp"

#include "mgl_engine/application.hpp"
#include "mgl_engine/layers/gui.hpp"

#include "imgui/imgui.h"

class imgui_game_layer : public mgl::engine::layers::gui_layer
{
  public:
  imgui_game_layer()
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
    config().gui_layer = mgl::create_ref<imgui_game_layer>();
  }
};

static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void imgui_game_layer::draw_ui(float time, float frame_time)
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
}

int main(int argc, char* argv[])
{
  game app;
  app.run();

  return 0;
}