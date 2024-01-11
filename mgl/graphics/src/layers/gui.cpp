#include "mgl_graphics/layers/gui.hpp"
#include "imgui/imgui.h"
#include "mgl_core/debug.hpp"
#include "mgl_graphics/render.hpp"
#include "mgl_window/event.hpp"
#include "mgl_window/input.hpp"
#include "mgl_window/window.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "shaders/fragment/imgui.hpp"
#include "shaders/vertex/imgui.hpp"
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

    auto bd = new imgui_render_data();

    if(!mgl::window::is_window_available())
    {
      MGL_CORE_ERROR("No window available");
      delete bd;
      return;
    }

    auto& render = mgl::graphics::current_render();
    auto ctx = render.context();
    ctx->enter();

    // Load vertex and fragment shaders from generated source
    mgl::opengl::shaders glsl = { vertex_shader_source, fragment_shader_source };

    bd->prg = ctx->program(glsl);
    if(!bd->prg)
    {
      MGL_CORE_ERROR("Failed to create ImGui shader program");
      delete bd;
      return;
    }

    bd->u_prj_matrix = bd->prg->uniform("ProjMtx");
    bd->u_position = bd->prg->uniform("Position");
    bd->u_uv = bd->prg->uniform("UV");
    bd->u_color = bd->prg->uniform("Color");
    bd->u_texture = bd->prg->uniform("Texture");
    (*bd->prg)["Texture"]->set_value(0);

    bd->vb = ctx->buffer();
    if(!bd->vb)
    {
      MGL_CORE_ERROR("Failed to create ImGui vertex buffer");
      bd->prg->release();
      delete bd;
      return;
    }

    bd->ib = ctx->buffer();
    if(!bd->ib)
    {
      MGL_CORE_ERROR("Failed to create ImGui index buffer");
      bd->vb->release();
      bd->prg->release();
      delete bd;
      return;
    }

    // Build texture atlas
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(
        &pixels,
        &width,
        &height); // Load as RGBA 32-bit (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

    bd->font_texture = ctx->texture2d(width, height, 4, pixels);
    if(!bd->font_texture)
    {
      MGL_CORE_ERROR("Failed to create ImGui font texture");
      bd->ib->release();
      bd->vb->release();
      bd->prg->release();
      delete bd;
      return;
    }

    io.Fonts->SetTexID((ImTextureID)(intptr_t)bd->font_texture->glo());

    io.BackendRendererUserData = bd;
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

  void gui_layer::shutdown_subsystem()
  {
    MGL_CORE_ASSERT(ImGui::GetCurrentContext() != nullptr, "ImGui Context not initialized");

    ImGuiIO& io = ImGui::GetIO();
    MGL_CORE_ASSERT(io.BackendRendererUserData != nullptr, "Backend not initialized");

    auto bd = static_cast<imgui_render_data*>(io.BackendRendererUserData);

    bd->ib->release();
    bd->vb->release();
    bd->prg->release();
    bd->u_prj_matrix = nullptr;
    bd->u_position = nullptr;
    bd->u_uv = nullptr;
    bd->u_color = nullptr;
    bd->u_texture = nullptr;

    io.BackendRendererUserData = nullptr;
    io.BackendRendererName = nullptr;
    io.BackendFlags &= ~ImGuiBackendFlags_RendererHasVtxOffset;

    io.Fonts->SetTexID(0);

    delete bd;

    ImGui::DestroyContext();
  }

  bool gui_layer::is_initialized()
  {
    return ImGui::GetCurrentContext() != nullptr;
  }

  void gui_layer::render_subsystem()
  {
    ImGuiIO& io = ImGui::GetIO();
    MGL_CORE_ASSERT(io.BackendRendererUserData != nullptr, "Not initialized");

    auto bd = static_cast<imgui_render_data*>(io.BackendRendererUserData);

    int fb_width = static_cast<int>(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height = static_cast<int>(io.DisplaySize.y * io.DisplayFramebufferScale.y);

    if(fb_width == 0 || fb_height == 0)
      return;

    ImGui::Render();

    ImDrawData* draw_data = ImGui::GetDrawData();

    if(!draw_data)
      return;

    bd->u_prj_matrix->set_value(
        glm::ortho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, -1.0f, 1.0f));

    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    auto& render = mgl::graphics::current_render();
    auto ctx = render.context();
    ctx->enter();

    ctx->enable(mgl::opengl::enable_flag::BLEND);
    ctx->set_blend_equation(mgl::opengl::blend_equation_mode::ADD);
    ctx->set_blend_func(mgl::opengl::blend_factor::SRC_ALPHA,
                        mgl::opengl::blend_factor::ONE_MINUS_SRC_ALPHA);

    bd->font_texture->use();

    ctx->enable_scissor();

    // Create temporary vertex array
    mgl::opengl::vertex_array_ref vao =
        ctx->vertex_array(bd->prg,
                          { { bd->vb, "2f 2f 4f1", { "Position", "UV", "Color" } } },
                          bd->ib,
                          sizeof(ImDrawIdx));

    for(int n = 0; n < draw_data->CmdListsCount; ++n)
    {
      const ImDrawList* cmd_list = draw_data->CmdLists[n];

      bd->vb->orphan(cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
      bd->ib->orphan(cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));

      auto r1 =
          bd->vb->write(cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
      auto r2 =
          bd->ib->write(cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));

      int idx_buffer_offset = 0;
      for(int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; ++cmd_i)
      {
        const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
        ctx->set_scissor(static_cast<int>(pcmd->ClipRect.x),
                         static_cast<int>(fb_height - pcmd->ClipRect.w),
                         static_cast<int>(pcmd->ClipRect.z - pcmd->ClipRect.x),
                         static_cast<int>(pcmd->ClipRect.w - pcmd->ClipRect.y));
        vao->render(mgl::opengl::render_mode::TRIANGLES, pcmd->ElemCount, idx_buffer_offset);
        idx_buffer_offset += pcmd->ElemCount;
      }
    }

    ctx->disable_scissor();

    // Cleanup temporary vertex array
    vao->release();
  }

  bool gui_layer::on_window_close(mgl::window::window_close_event& event)
  {
    return false;
  }

  bool gui_layer::on_window_resize(mgl::window::window_resize_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    MGL_CORE_ASSERT(io.BackendRendererUserData != nullptr, "Not initialized");
    const auto& e = static_cast<const mgl::window::window_resize_event&>(event);
    io.DisplaySize = ImVec2(e.width(), e.height());
    return false;
  }

  bool gui_layer::on_key_pressed(mgl::window::key_pressed_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    MGL_CORE_ASSERT(io.BackendRendererUserData != nullptr, "Not initialized");
    const auto& e = static_cast<const mgl::window::key_pressed_event&>(event);
    io.KeysDown[static_cast<int>(e.key())] = true;
    return false;
  }

  bool gui_layer::on_key_released(mgl::window::key_released_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    MGL_CORE_ASSERT(io.BackendRendererUserData != nullptr, "Not initialized");
    const auto& e = static_cast<const mgl::window::key_released_event&>(event);
    io.KeysDown[static_cast<int>(e.key())] = false;
    return false;
  }

  bool gui_layer::on_mouse_moved(mgl::window::mouse_moved_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    MGL_CORE_ASSERT(io.BackendRendererUserData != nullptr, "Not initialized");
    const auto& e = static_cast<const mgl::window::mouse_moved_event&>(event);
    io.MousePos = ImVec2(e.x(), e.y());
    return false;
  }

  bool gui_layer::on_mouse_scrolled(mgl::window::mouse_scrolled_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    MGL_CORE_ASSERT(io.BackendRendererUserData != nullptr, "Not initialized");
    const auto& e = static_cast<const mgl::window::mouse_scrolled_event&>(event);
    io.MouseWheelH += e.x_offset();
    io.MouseWheel += e.y_offset();
    return false;
  }

  bool gui_layer::on_mouse_button_pressed(mgl::window::mouse_button_pressed_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    MGL_CORE_ASSERT(io.BackendRendererUserData != nullptr, "Not initialized");
    const auto& e = static_cast<const mgl::window::mouse_button_pressed_event&>(event);
    io.MouseDown[static_cast<int>(e.button())] = true;
    return false;
  }

  bool gui_layer::on_mouse_button_released(mgl::window::mouse_button_released_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    MGL_CORE_ASSERT(io.BackendRendererUserData != nullptr, "Not initialized");
    const auto& e = static_cast<const mgl::window::mouse_button_released_event&>(event);
    io.MouseDown[static_cast<int>(e.button())] = false;
    return false;
  }

} // namespace mgl::graphics::layers
