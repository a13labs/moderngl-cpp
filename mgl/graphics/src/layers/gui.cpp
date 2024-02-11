#include "mgl_graphics/layers/gui.hpp"
#include "mgl_graphics/graphics.hpp"
#include "mgl_graphics/shaders/gui.hpp"
#include "mgl_graphics/textures/texture2d.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/profiling.hpp"
#include "mgl_platform/event.hpp"
#include "mgl_platform/input.hpp"
#include "mgl_platform/window.hpp"
#include "mgl_registry/resources/image.hpp"

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
    MGL_PROFILE_FUNCTION("GUI_LAYER");
    draw_ui(time, frame_time);
    render_subsystem();
  }

  void gui_layer::on_event(mgl::platform::event& event)
  {
    if(!is_initialized())
      return;

    mgl::platform::EventDispatcher dispatcher(event);

    // Dispatch Windows Events
    dispatcher.dispatch<mgl::platform::window_close_event>(
        MGL_BIND_EVENT_FN(gui_layer::on_window_close));

    dispatcher.dispatch<mgl::platform::window_resize_event>(
        MGL_BIND_EVENT_FN(gui_layer::on_window_resize));

    // Dispatch key events to be handled by the application
    dispatcher.dispatch<mgl::platform::key_pressed_event>(
        MGL_BIND_EVENT_FN(gui_layer::on_key_pressed));

    dispatcher.dispatch<mgl::platform::key_released_event>(
        MGL_BIND_EVENT_FN(gui_layer::on_key_released));

    // Dispatch mouse events to be handled by the application
    dispatcher.dispatch<mgl::platform::mouse_moved_event>(
        MGL_BIND_EVENT_FN(gui_layer::on_mouse_moved));

    dispatcher.dispatch<mgl::platform::mouse_scrolled_event>(
        MGL_BIND_EVENT_FN(gui_layer::on_mouse_scrolled));

    dispatcher.dispatch<mgl::platform::mouse_button_pressed_event>(
        MGL_BIND_EVENT_FN(gui_layer::on_mouse_button_pressed));

    dispatcher.dispatch<mgl::platform::mouse_button_released_event>(
        MGL_BIND_EVENT_FN(gui_layer::on_mouse_button_released));
  }

  void gui_layer::init_subsystem()
  {
    MGL_CORE_ASSERT(ImGui::GetCurrentContext() == nullptr, "Already initialized");

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    MGL_CORE_ASSERT(io.BackendRendererUserData == nullptr,
                    "Backend already initialized, shutdown first");

    if(!mgl::platform::is_window_available())
    {
      MGL_CORE_ERROR("No window available");
      return;
    }

    register_shader("gui", mgl::create_ref<builtins::gui_shader>());
    register_buffer("gui_vb", mgl::create_ref<vertex_buffer>("2f 2f 4f1", 0, true));
    register_buffer("gui_ib", mgl::create_ref<index_buffer>(0, sizeof(ImDrawIdx), true));

    refresh_font();

    io.BackendRendererName = "mgl::platform::imgui";
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

    io.KeyMap[ImGuiKey_Tab] = static_cast<int>(mgl::platform::key::Tab);
    io.KeyMap[ImGuiKey_LeftArrow] = static_cast<int>(mgl::platform::key::Left);
    io.KeyMap[ImGuiKey_RightArrow] = static_cast<int>(mgl::platform::key::Right);
    io.KeyMap[ImGuiKey_UpArrow] = static_cast<int>(mgl::platform::key::Up);
    io.KeyMap[ImGuiKey_DownArrow] = static_cast<int>(mgl::platform::key::Down);
    io.KeyMap[ImGuiKey_PageUp] = static_cast<int>(mgl::platform::key::PageUp);
    io.KeyMap[ImGuiKey_PageDown] = static_cast<int>(mgl::platform::key::PageDown);
    io.KeyMap[ImGuiKey_Home] = static_cast<int>(mgl::platform::key::Home);
    io.KeyMap[ImGuiKey_End] = static_cast<int>(mgl::platform::key::End);
    io.KeyMap[ImGuiKey_Insert] = static_cast<int>(mgl::platform::key::Insert);
    io.KeyMap[ImGuiKey_Delete] = static_cast<int>(mgl::platform::key::Delete);
    io.KeyMap[ImGuiKey_Backspace] = static_cast<int>(mgl::platform::key::Backspace);
    io.KeyMap[ImGuiKey_Space] = static_cast<int>(mgl::platform::key::Space);
    io.KeyMap[ImGuiKey_Enter] = static_cast<int>(mgl::platform::key::Return);
    io.KeyMap[ImGuiKey_Escape] = static_cast<int>(mgl::platform::key::Esc);
    io.KeyMap[ImGuiKey_KeyPadEnter] = static_cast<int>(mgl::platform::key::NumPadEnter);
    io.KeyMap[ImGuiKey_A] = static_cast<int>(mgl::platform::key::KeyA);
    io.KeyMap[ImGuiKey_B] = static_cast<int>(mgl::platform::key::KeyB);
    io.KeyMap[ImGuiKey_C] = static_cast<int>(mgl::platform::key::KeyC);
    io.KeyMap[ImGuiKey_D] = static_cast<int>(mgl::platform::key::KeyD);
    io.KeyMap[ImGuiKey_E] = static_cast<int>(mgl::platform::key::KeyE);
    io.KeyMap[ImGuiKey_F] = static_cast<int>(mgl::platform::key::KeyF);
    io.KeyMap[ImGuiKey_G] = static_cast<int>(mgl::platform::key::KeyG);
    io.KeyMap[ImGuiKey_H] = static_cast<int>(mgl::platform::key::KeyH);
    io.KeyMap[ImGuiKey_I] = static_cast<int>(mgl::platform::key::KeyI);
    io.KeyMap[ImGuiKey_J] = static_cast<int>(mgl::platform::key::KeyJ);
    io.KeyMap[ImGuiKey_K] = static_cast<int>(mgl::platform::key::KeyK);
    io.KeyMap[ImGuiKey_L] = static_cast<int>(mgl::platform::key::KeyL);
    io.KeyMap[ImGuiKey_M] = static_cast<int>(mgl::platform::key::KeyM);
    io.KeyMap[ImGuiKey_N] = static_cast<int>(mgl::platform::key::KeyN);
    io.KeyMap[ImGuiKey_O] = static_cast<int>(mgl::platform::key::KeyO);
    io.KeyMap[ImGuiKey_P] = static_cast<int>(mgl::platform::key::KeyP);
    io.KeyMap[ImGuiKey_Q] = static_cast<int>(mgl::platform::key::KeyQ);
    io.KeyMap[ImGuiKey_R] = static_cast<int>(mgl::platform::key::KeyR);
    io.KeyMap[ImGuiKey_S] = static_cast<int>(mgl::platform::key::KeyS);
    io.KeyMap[ImGuiKey_T] = static_cast<int>(mgl::platform::key::KeyT);
    io.KeyMap[ImGuiKey_U] = static_cast<int>(mgl::platform::key::KeyU);
    io.KeyMap[ImGuiKey_V] = static_cast<int>(mgl::platform::key::KeyV);
    io.KeyMap[ImGuiKey_W] = static_cast<int>(mgl::platform::key::KeyW);
    io.KeyMap[ImGuiKey_X] = static_cast<int>(mgl::platform::key::KeyX);
    io.KeyMap[ImGuiKey_Y] = static_cast<int>(mgl::platform::key::KeyY);
    io.KeyMap[ImGuiKey_Z] = static_cast<int>(mgl::platform::key::KeyZ);
    io.KeyMap[ImGuiKey_LeftShift] = static_cast<int>(mgl::platform::key::LeftShift);
    io.KeyMap[ImGuiKey_RightShift] = static_cast<int>(mgl::platform::key::RightShift);
    io.KeyMap[ImGuiKey_LeftCtrl] = static_cast<int>(mgl::platform::key::LeftCtrl);
    io.KeyMap[ImGuiKey_RightCtrl] = static_cast<int>(mgl::platform::key::RightCtrl);
    io.KeyMap[ImGuiKey_LeftAlt] = static_cast<int>(mgl::platform::key::LeftAlt);
    io.KeyMap[ImGuiKey_RightAlt] = static_cast<int>(mgl::platform::key::RightAlt);
    io.KeyMap[ImGuiKey_0] = static_cast<int>(mgl::platform::key::Key0);
    io.KeyMap[ImGuiKey_1] = static_cast<int>(mgl::platform::key::Key1);
    io.KeyMap[ImGuiKey_2] = static_cast<int>(mgl::platform::key::Key2);
    io.KeyMap[ImGuiKey_3] = static_cast<int>(mgl::platform::key::Key3);
    io.KeyMap[ImGuiKey_4] = static_cast<int>(mgl::platform::key::Key4);
    io.KeyMap[ImGuiKey_5] = static_cast<int>(mgl::platform::key::Key5);
    io.KeyMap[ImGuiKey_6] = static_cast<int>(mgl::platform::key::Key6);
    io.KeyMap[ImGuiKey_7] = static_cast<int>(mgl::platform::key::Key7);
    io.KeyMap[ImGuiKey_8] = static_cast<int>(mgl::platform::key::Key8);
    io.KeyMap[ImGuiKey_9] = static_cast<int>(mgl::platform::key::Key9);
    io.KeyMap[ImGuiKey_F1] = static_cast<int>(mgl::platform::key::F1);
    io.KeyMap[ImGuiKey_F2] = static_cast<int>(mgl::platform::key::F2);
    io.KeyMap[ImGuiKey_F3] = static_cast<int>(mgl::platform::key::F3);
    io.KeyMap[ImGuiKey_F4] = static_cast<int>(mgl::platform::key::F4);
    io.KeyMap[ImGuiKey_F5] = static_cast<int>(mgl::platform::key::F5);
    io.KeyMap[ImGuiKey_F6] = static_cast<int>(mgl::platform::key::F6);
    io.KeyMap[ImGuiKey_F7] = static_cast<int>(mgl::platform::key::F7);
    io.KeyMap[ImGuiKey_F8] = static_cast<int>(mgl::platform::key::F8);
    io.KeyMap[ImGuiKey_F9] = static_cast<int>(mgl::platform::key::F9);
    io.KeyMap[ImGuiKey_F10] = static_cast<int>(mgl::platform::key::F10);
    io.KeyMap[ImGuiKey_F11] = static_cast<int>(mgl::platform::key::F11);
    io.KeyMap[ImGuiKey_F12] = static_cast<int>(mgl::platform::key::F12);

    io.DisplaySize =
        ImVec2(mgl::platform::current_window().width(), mgl::platform::current_window().height());
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

    if(has_texture("gui_font"))
      unregister_texture("gui_font");

    size_t id = register_texture("gui_font", mgl::create_ref<mgl::graphics::texture2d>(image));
    io.Fonts->TexID = reinterpret_cast<void*>(id);
  }

  void gui_layer::shutdown_subsystem()
  {
    MGL_CORE_ASSERT(ImGui::GetCurrentContext() != nullptr, "ImGui Context not initialized");

    ImGuiIO& io = ImGui::GetIO();

    unregister_buffer("gui_ib");
    unregister_buffer("gui_vb");
    unregister_shader("gui");
    unregister_texture("gui_font");

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
    MGL_PROFILE_FUNCTION("GUI_LAYER");
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

    auto prg = get_shader("gui");

    MGL_CORE_ASSERT(prg != nullptr, "No shader available");

    mgl::platform::api::enable_program(prg->api());
    mgl::platform::api::set_program_attributes(prg->attributes());

    prg->set_uniform_value("ProjMtx",
                           glm::ortho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, -1.0f, 1.0f));

    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    mgl::platform::api::enable_state(mgl::graphics::enable_flag::BLEND);
    mgl::platform::api::set_blend_equation(mgl::graphics::blend_equation_mode::ADD);
    mgl::platform::api::set_blend_func(mgl::graphics::blend_factor::SRC_ALPHA,
                                       mgl::graphics::blend_factor::ONE_MINUS_SRC_ALPHA);

    mgl::platform::api::enable_scissor();

    auto vb = std::static_pointer_cast<vertex_buffer>(get_buffer("gui_vb"));
    auto ib = std::static_pointer_cast<index_buffer>(get_buffer("gui_ib"));

    for(int n = 0; n < draw_data->CmdListsCount; ++n)
    {
      const ImDrawList* cmd_list = draw_data->CmdLists[n];

      vb->orphan(cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
      ib->orphan(cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));

      vb->upload(cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
      ib->upload(cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));

      mgl::platform::api::vertex_array_ref vao =
          mgl::platform::api::create_vertex_array(vb->api(),
                                                  vb->layout(),
                                                  ib->api(),
                                                  ib->element_size(),
                                                  (uint32_t)mgl::graphics::render_mode::TRIANGLES);

      vao->allocate();

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
          mgl::platform::api::set_scissor(static_cast<int>(pcmd->ClipRect.x),
                                          static_cast<int>(fb_height - pcmd->ClipRect.w),
                                          static_cast<int>(pcmd->ClipRect.z - pcmd->ClipRect.x),
                                          static_cast<int>(pcmd->ClipRect.w - pcmd->ClipRect.y));

          auto tex = get_texture(reinterpret_cast<size_t>(pcmd->TextureId));
          tex->bind(0);

          mgl::platform::api::draw(vao, glm::mat4(1.0f), pcmd->ElemCount, idx_buffer_offset, 0);

          idx_buffer_offset += pcmd->ElemCount;
        }
      }

      vao->deallocate();
    }

    mgl::platform::api::clear_samplers(0, 1);
    mgl::platform::api::disable_program();
    mgl::platform::api::disable_scissor();
  }

  bool gui_layer::on_window_close(mgl::platform::window_close_event& event)
  {
    return false;
  }

  bool gui_layer::on_window_resize(mgl::platform::window_resize_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    const auto& e = static_cast<const mgl::platform::window_resize_event&>(event);
    io.DisplaySize = ImVec2(e.width(), e.height());
    return false;
  }

  bool gui_layer::on_key_pressed(mgl::platform::key_pressed_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    const auto& e = static_cast<const mgl::platform::key_pressed_event&>(event);
    io.KeysDown[static_cast<int>(e.key())] = true;
    return false;
  }

  bool gui_layer::on_key_released(mgl::platform::key_released_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    const auto& e = static_cast<const mgl::platform::key_released_event&>(event);
    io.KeysDown[static_cast<int>(e.key())] = false;
    return false;
  }

  bool gui_layer::on_mouse_moved(mgl::platform::mouse_moved_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    const auto& e = static_cast<const mgl::platform::mouse_moved_event&>(event);
    io.MousePos = ImVec2(e.x(), e.y());
    return false;
  }

  bool gui_layer::on_mouse_scrolled(mgl::platform::mouse_scrolled_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    const auto& e = static_cast<const mgl::platform::mouse_scrolled_event&>(event);
    io.MouseWheelH += e.x_offset();
    io.MouseWheel += e.y_offset();
    return false;
  }

  bool gui_layer::on_mouse_button_pressed(mgl::platform::mouse_button_pressed_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    const auto& e = static_cast<const mgl::platform::mouse_button_pressed_event&>(event);
    io.MouseDown[static_cast<int>(e.button())] = true;
    return false;
  }

  bool gui_layer::on_mouse_button_released(mgl::platform::mouse_button_released_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    const auto& e = static_cast<const mgl::platform::mouse_button_released_event&>(event);
    io.MouseDown[static_cast<int>(e.button())] = false;
    return false;
  }

} // namespace mgl::graphics::layers
