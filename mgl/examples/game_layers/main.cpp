
#include "mgl_core/memory.hpp"
#include "mgl_game/layers/imgui_layer.hpp"
#include "mgl_game/window.hpp"
#include "mgl_opengl/context.hpp"

#include "imgui/imgui.h"

class imgui_game_layer : public mgl::game::layers::imgui_layer
{
  public:
  imgui_game_layer()
      : mgl::game::layers::imgui_layer("ImGui Layer")
  { }
  virtual void draw_ui(float time, float frame_time) override;
};

class game_window : public mgl::game::window
{
  public:
  virtual void on_load() override;
  virtual void on_unload() override;
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

void game_window::on_load()
{
  set_title("Layers Example");
  layers().push_back(mgl::create_ref<imgui_game_layer>());
}

void game_window::on_unload() { }

int main(int argc, char* argv[])
{
  game_window app;
  app.run();

  return 0;
}