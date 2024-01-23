#include "layers.hpp"
#include "mgl_application/application.hpp"

class example : public mgl::application::application
{
  public:
  example()
      : mgl::application::application()
  {
    auto gui = mgl::create_ref<gui_layer>();
    auto render = mgl::create_ref<render_layer>();

    config().gui_layer = gui;
    config().render_layer = render;
  }
};

int main(int argc, char* argv[])
{
  example app;
  app.run();

  return 0;
}