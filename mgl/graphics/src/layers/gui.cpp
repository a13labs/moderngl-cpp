#include "mgl_graphics/layers/gui.hpp"
#include "mgl_graphics/render.hpp"
#include "mgl_graphics/shaders/gui.hpp"
#include "mgl_graphics/textures/texture2d.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_registry/resources/image.hpp"
#include "mgl_window/event.hpp"
#include "mgl_window/input.hpp"
#include "mgl_window/window.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
namespace mgl::graphics::layers
{
  gui_layer::gui_layer(const std::string& name)
      : layer(name)
  { }

  gui_layer::~gui_layer() { }

  void gui_layer::on_attach()
  {
    init_subsystem();
  }

  void gui_layer::on_detach()
  {
    shutdown_subsystem();
  }

  void gui_layer::on_update(float time, float frame_time)
  {
    draw_ui(time, frame_time);
    render_subsystem();
  }

  void gui_layer::on_event(mgl::window::event& event)
  {
    if(!is_initialized())
      return;

    mgl::window::EventDispatcher dispatcher(event);

    // Dispatch Windows Events
    dispatcher.dispatch<mgl::window::window_close_event>(
        MGL_BIND_EVENT_FN(gui_layer::on_window_close));

    dispatcher.dispatch<mgl::window::window_resize_event>(
        MGL_BIND_EVENT_FN(gui_layer::on_window_resize));

    // Dispatch key events to be handled by the application
    dispatcher.dispatch<mgl::window::key_pressed_event>(
        MGL_BIND_EVENT_FN(gui_layer::on_key_pressed));

    dispatcher.dispatch<mgl::window::key_released_event>(
        MGL_BIND_EVENT_FN(gui_layer::on_key_released));

    // Dispatch mouse events to be handled by the application
    dispatcher.dispatch<mgl::window::mouse_moved_event>(
        MGL_BIND_EVENT_FN(gui_layer::on_mouse_moved));

    dispatcher.dispatch<mgl::window::mouse_scrolled_event>(
        MGL_BIND_EVENT_FN(gui_layer::on_mouse_scrolled));

    dispatcher.dispatch<mgl::window::mouse_button_pressed_event>(
        MGL_BIND_EVENT_FN(gui_layer::on_mouse_button_pressed));

    dispatcher.dispatch<mgl::window::mouse_button_released_event>(
        MGL_BIND_EVENT_FN(gui_layer::on_mouse_button_released));
  }

  void gui_layer::init_subsystem()
  {
    MGL_CORE_ASSERT(ImGui::GetCurrentContext() == nullptr, "Already initialized");

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    MGL_CORE_ASSERT(io.BackendRendererUserData == nullptr,
                    "Backend already initialized, shutdown first");

    if(!mgl::window::is_window_available())
    {
      MGL_CORE_ERROR("No window available");
      return;
    }

    auto& render = mgl::graphics::current_render();
    render.register_shader("gui", mgl::create_ref<shaders::gui>());
    render.register_buffer("gui_vb", mgl::create_ref<vertex_buffer>("2f 2f 4f1", 0, true));
    render.register_buffer("gui_ib", mgl::create_ref<index_buffer>(0, sizeof(ImDrawIdx), true));

    refresh_font();

    io.BackendRendererName = "mgl::window::imgui";
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

    io.KeyMap[ImGuiKey_Tab] = static_cast<int>(mgl::window::key::Tab);
    io.KeyMap[ImGuiKey_LeftArrow] = static_cast<int>(mgl::window::key::Left);
    io.KeyMap[ImGuiKey_RightArrow] = static_cast<int>(mgl::window::key::Right);
    io.KeyMap[ImGuiKey_UpArrow] = static_cast<int>(mgl::window::key::Up);
    io.KeyMap[ImGuiKey_DownArrow] = static_cast<int>(mgl::window::key::Down);
    io.KeyMap[ImGuiKey_PageUp] = static_cast<int>(mgl::window::key::PageUp);
    io.KeyMap[ImGuiKey_PageDown] = static_cast<int>(mgl::window::key::PageDown);
    io.KeyMap[ImGuiKey_Home] = static_cast<int>(mgl::window::key::Home);
    io.KeyMap[ImGuiKey_End] = static_cast<int>(mgl::window::key::End);
    io.KeyMap[ImGuiKey_Insert] = static_cast<int>(mgl::window::key::Insert);
    io.KeyMap[ImGuiKey_Delete] = static_cast<int>(mgl::window::key::Delete);
    io.KeyMap[ImGuiKey_Backspace] = static_cast<int>(mgl::window::key::Backspace);
    io.KeyMap[ImGuiKey_Space] = static_cast<int>(mgl::window::key::Space);
    io.KeyMap[ImGuiKey_Enter] = static_cast<int>(mgl::window::key::Return);
    io.KeyMap[ImGuiKey_Escape] = static_cast<int>(mgl::window::key::Esc);
    io.KeyMap[ImGuiKey_KeyPadEnter] = static_cast<int>(mgl::window::key::NumPadEnter);
    io.KeyMap[ImGuiKey_A] = static_cast<int>(mgl::window::key::KeyA);
    io.KeyMap[ImGuiKey_B] = static_cast<int>(mgl::window::key::KeyB);
    io.KeyMap[ImGuiKey_C] = static_cast<int>(mgl::window::key::KeyC);
    io.KeyMap[ImGuiKey_D] = static_cast<int>(mgl::window::key::KeyD);
    io.KeyMap[ImGuiKey_E] = static_cast<int>(mgl::window::key::KeyE);
    io.KeyMap[ImGuiKey_F] = static_cast<int>(mgl::window::key::KeyF);
    io.KeyMap[ImGuiKey_G] = static_cast<int>(mgl::window::key::KeyG);
    io.KeyMap[ImGuiKey_H] = static_cast<int>(mgl::window::key::KeyH);
    io.KeyMap[ImGuiKey_I] = static_cast<int>(mgl::window::key::KeyI);
    io.KeyMap[ImGuiKey_J] = static_cast<int>(mgl::window::key::KeyJ);
    io.KeyMap[ImGuiKey_K] = static_cast<int>(mgl::window::key::KeyK);
    io.KeyMap[ImGuiKey_L] = static_cast<int>(mgl::window::key::KeyL);
    io.KeyMap[ImGuiKey_M] = static_cast<int>(mgl::window::key::KeyM);
    io.KeyMap[ImGuiKey_N] = static_cast<int>(mgl::window::key::KeyN);
    io.KeyMap[ImGuiKey_O] = static_cast<int>(mgl::window::key::KeyO);
    io.KeyMap[ImGuiKey_P] = static_cast<int>(mgl::window::key::KeyP);
    io.KeyMap[ImGuiKey_Q] = static_cast<int>(mgl::window::key::KeyQ);
    io.KeyMap[ImGuiKey_R] = static_cast<int>(mgl::window::key::KeyR);
    io.KeyMap[ImGuiKey_S] = static_cast<int>(mgl::window::key::KeyS);
    io.KeyMap[ImGuiKey_T] = static_cast<int>(mgl::window::key::KeyT);
    io.KeyMap[ImGuiKey_U] = static_cast<int>(mgl::window::key::KeyU);
    io.KeyMap[ImGuiKey_V] = static_cast<int>(mgl::window::key::KeyV);
    io.KeyMap[ImGuiKey_W] = static_cast<int>(mgl::window::key::KeyW);
    io.KeyMap[ImGuiKey_X] = static_cast<int>(mgl::window::key::KeyX);
    io.KeyMap[ImGuiKey_Y] = static_cast<int>(mgl::window::key::KeyY);
    io.KeyMap[ImGuiKey_Z] = static_cast<int>(mgl::window::key::KeyZ);
    io.KeyMap[ImGuiKey_LeftShift] = static_cast<int>(mgl::window::key::LeftShift);
    io.KeyMap[ImGuiKey_RightShift] = static_cast<int>(mgl::window::key::RightShift);
    io.KeyMap[ImGuiKey_LeftCtrl] = static_cast<int>(mgl::window::key::LeftCtrl);
    io.KeyMap[ImGuiKey_RightCtrl] = static_cast<int>(mgl::window::key::RightCtrl);
    io.KeyMap[ImGuiKey_LeftAlt] = static_cast<int>(mgl::window::key::LeftAlt);
    io.KeyMap[ImGuiKey_RightAlt] = static_cast<int>(mgl::window::key::RightAlt);
    io.KeyMap[ImGuiKey_0] = static_cast<int>(mgl::window::key::Key0);
    io.KeyMap[ImGuiKey_1] = static_cast<int>(mgl::window::key::Key1);
    io.KeyMap[ImGuiKey_2] = static_cast<int>(mgl::window::key::Key2);
    io.KeyMap[ImGuiKey_3] = static_cast<int>(mgl::window::key::Key3);
    io.KeyMap[ImGuiKey_4] = static_cast<int>(mgl::window::key::Key4);
    io.KeyMap[ImGuiKey_5] = static_cast<int>(mgl::window::key::Key5);
    io.KeyMap[ImGuiKey_6] = static_cast<int>(mgl::window::key::Key6);
    io.KeyMap[ImGuiKey_7] = static_cast<int>(mgl::window::key::Key7);
    io.KeyMap[ImGuiKey_8] = static_cast<int>(mgl::window::key::Key8);
    io.KeyMap[ImGuiKey_9] = static_cast<int>(mgl::window::key::Key9);
    io.KeyMap[ImGuiKey_F1] = static_cast<int>(mgl::window::key::F1);
    io.KeyMap[ImGuiKey_F2] = static_cast<int>(mgl::window::key::F2);
    io.KeyMap[ImGuiKey_F3] = static_cast<int>(mgl::window::key::F3);
    io.KeyMap[ImGuiKey_F4] = static_cast<int>(mgl::window::key::F4);
    io.KeyMap[ImGuiKey_F5] = static_cast<int>(mgl::window::key::F5);
    io.KeyMap[ImGuiKey_F6] = static_cast<int>(mgl::window::key::F6);
    io.KeyMap[ImGuiKey_F7] = static_cast<int>(mgl::window::key::F7);
    io.KeyMap[ImGuiKey_F8] = static_cast<int>(mgl::window::key::F8);
    io.KeyMap[ImGuiKey_F9] = static_cast<int>(mgl::window::key::F9);
    io.KeyMap[ImGuiKey_F10] = static_cast<int>(mgl::window::key::F10);
    io.KeyMap[ImGuiKey_F11] = static_cast<int>(mgl::window::key::F11);
    io.KeyMap[ImGuiKey_F12] = static_cast<int>(mgl::window::key::F12);

    io.DisplaySize =
        ImVec2(mgl::window::current_window().width(), mgl::window::current_window().height());
    io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
  }

  void gui_layer::refresh_font()
  {
    ImGuiIO& io = ImGui::GetIO();

    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(
        &pixels,
        &width,
        &height); // Load as RGBA 32-bit (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

    auto image = mgl::create_ref<mgl::registry::image>(width, height, 4, pixels);
    auto& render = mgl::graphics::current_render();
    if(render.has_texture("gui_font"))
      render.unregister_texture("gui_font");

    uint32_t id =
        render.register_texture("gui_font", mgl::create_ref<mgl::graphics::texture2d>(image));
    io.Fonts->TexID = reinterpret_cast<void*>(id);
  }

  void gui_layer::shutdown_subsystem()
  {
    MGL_CORE_ASSERT(ImGui::GetCurrentContext() != nullptr, "ImGui Context not initialized");

    ImGuiIO& io = ImGui::GetIO();

    auto& render = mgl::graphics::current_render();

    render.unregister_buffer("gui_ib");
    render.unregister_buffer("gui_vb");
    render.unregister_shader("gui");
    render.unregister_texture("gui_font");

    io.BackendRendererUserData = nullptr;
    io.BackendRendererName = nullptr;
    io.BackendFlags &= ~ImGuiBackendFlags_RendererHasVtxOffset;

    ImGui::DestroyContext();
  }

  bool gui_layer::is_initialized()
  {
    return ImGui::GetCurrentContext() != nullptr;
  }

  void gui_layer::render_subsystem()
  {
    MGL_CORE_ASSERT(ImGui::GetCurrentContext() != nullptr, "ImGui Context not initialized");

    ImGuiIO& io = ImGui::GetIO();

    int fb_width = static_cast<int>(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height = static_cast<int>(io.DisplaySize.y * io.DisplayFramebufferScale.y);

    if(fb_width == 0 || fb_height == 0)
      return;

    ImGui::Render();

    ImDrawData* draw_data = ImGui::GetDrawData();

    if(!draw_data)
      return;

    auto& render = mgl::graphics::current_render();

    auto prg = render.get_shader("gui");

    MGL_CORE_ASSERT(prg != nullptr, "No shader available");

    render.current_state().current_shader = prg;
    prg->set_uniform_value("ProjMtx",
                           glm::ortho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, -1.0f, 1.0f));

    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    render.enable_state(mgl::graphics::enable_flag::BLEND);
    render.set_blend_equation(mgl::graphics::blend_equation_mode::ADD);
    render.set_blend_func(mgl::graphics::blend_factor::SRC_ALPHA,
                          mgl::graphics::blend_factor::ONE_MINUS_SRC_ALPHA);

    render.enable_scissor();

    auto vb = std::static_pointer_cast<vertex_buffer>(render.get_buffer("gui_vb"));
    auto ib = std::static_pointer_cast<index_buffer>(render.get_buffer("gui_ib"));

    for(int n = 0; n < draw_data->CmdListsCount; ++n)
    {
      const ImDrawList* cmd_list = draw_data->CmdLists[n];

      vb->orphan(cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
      ib->orphan(cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));

      vb->upload(cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
      ib->upload(cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));

      int idx_buffer_offset = 0;
      for(int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; ++cmd_i)
      {
        const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];

        if(pcmd->UserCallback)
        {
          pcmd->UserCallback(cmd_list, pcmd);
        }
        else
        {
          render.set_scissor(static_cast<int>(pcmd->ClipRect.x),
                             static_cast<int>(fb_height - pcmd->ClipRect.w),
                             static_cast<int>(pcmd->ClipRect.z - pcmd->ClipRect.x),
                             static_cast<int>(pcmd->ClipRect.w - pcmd->ClipRect.y));

          auto tex = render.get_texture(reinterpret_cast<size_t>(pcmd->TextureId));
          tex->bind(0);

          render.draw(vb,
                      ib,
                      mgl::graphics::render_mode::TRIANGLES,
                      glm::mat4(1.0f),
                      pcmd->ElemCount,
                      idx_buffer_offset);
          idx_buffer_offset += pcmd->ElemCount;
        }
      }
    }

    render.clear_samplers(0, 1);

    prg->unbind();

    render.disable_scissor();
  }

  bool gui_layer::on_window_close(mgl::window::window_close_event& event)
  {
    return false;
  }

  bool gui_layer::on_window_resize(mgl::window::window_resize_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    const auto& e = static_cast<const mgl::window::window_resize_event&>(event);
    io.DisplaySize = ImVec2(e.width(), e.height());
    return false;
  }

  bool gui_layer::on_key_pressed(mgl::window::key_pressed_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    const auto& e = static_cast<const mgl::window::key_pressed_event&>(event);
    io.KeysDown[static_cast<int>(e.key())] = true;
    return false;
  }

  bool gui_layer::on_key_released(mgl::window::key_released_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    const auto& e = static_cast<const mgl::window::key_released_event&>(event);
    io.KeysDown[static_cast<int>(e.key())] = false;
    return false;
  }

  bool gui_layer::on_mouse_moved(mgl::window::mouse_moved_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    const auto& e = static_cast<const mgl::window::mouse_moved_event&>(event);
    io.MousePos = ImVec2(e.x(), e.y());
    return false;
  }

  bool gui_layer::on_mouse_scrolled(mgl::window::mouse_scrolled_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    const auto& e = static_cast<const mgl::window::mouse_scrolled_event&>(event);
    io.MouseWheelH += e.x_offset();
    io.MouseWheel += e.y_offset();
    return false;
  }

  bool gui_layer::on_mouse_button_pressed(mgl::window::mouse_button_pressed_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    const auto& e = static_cast<const mgl::window::mouse_button_pressed_event&>(event);
    io.MouseDown[static_cast<int>(e.button())] = true;
    return false;
  }

  bool gui_layer::on_mouse_button_released(mgl::window::mouse_button_released_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    const auto& e = static_cast<const mgl::window::mouse_button_released_event&>(event);
    io.MouseDown[static_cast<int>(e.button())] = false;
    return false;
  }

} // namespace mgl::graphics::layers
