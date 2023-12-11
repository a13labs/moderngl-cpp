#include "mgl_game/integrations/imgui.hpp"
#include "mgl_game/input.hpp"
#include "mgl_game/window.hpp"

#include "mgl_opengl/context.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/string.hpp"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>

namespace mgl::game::imgui
{

  const std::string s_vertex_shader = R"(
        #version 330
        uniform mat4 ProjMtx;
        in vec2 Position;
        in vec2 UV;
        in vec4 Color;
        out vec2 Frag_UV;
        out vec4 Frag_Color;
        void main() {
            Frag_UV = UV;
            Frag_Color = Color;
            gl_Position = ProjMtx * vec4(Position.xy, 0, 1);
        }
    )";

  const std::string s_fragment_shader = R"(
        #version 330
        uniform sampler2D Texture;
        in vec2 Frag_UV;
        in vec4 Frag_Color;
        out vec4 Out_Color;
        void main() {
            Out_Color = Frag_Color * texture(Texture, Frag_UV.st);
        }
    )";

  struct imgui_render_data
  {
    mgl::opengl::program_ref prg;
    mgl::opengl::uniform_ref u_prj_matrix;
    mgl::opengl::uniform_ref u_position;
    mgl::opengl::uniform_ref u_uv;
    mgl::opengl::uniform_ref u_texture;
    mgl::opengl::uniform_ref u_color;
    mgl::opengl::buffer_ref vb;
    mgl::opengl::buffer_ref ib;
    mgl::opengl::texture_2d_ref font_texture;

    imgui_render_data()
        : prg(nullptr)
        , u_prj_matrix(nullptr)
        , u_position(nullptr)
        , u_uv(nullptr)
        , u_texture(nullptr)
        , u_color(nullptr)
        , vb(nullptr)
        , ib(nullptr)
        , font_texture(nullptr)
    { }
  };

  bool on_window_close(window_close_event& event);
  bool on_window_resize(window_resize_event& event);
  bool on_key_pressed(key_pressed_event& event);
  bool on_key_released(key_released_event& event);
  bool on_mouse_moved(mouse_moved_event& event);
  bool on_mouse_scrolled(mouse_scrolled_event& event);
  bool on_mouse_button_pressed(mouse_button_pressed_event& event);
  bool on_mouse_button_released(mouse_button_released_event& event);

  void initialize()
  {
    MGL_CORE_ASSERT(ImGui::GetCurrentContext() == nullptr, "Already initialized");

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    MGL_CORE_ASSERT(io.BackendRendererUserData == nullptr,
                    "Backend already initialized, shutdown first");

    mgl::game::current_context()->enter();
    auto bd = new imgui_render_data();
    auto ctx = mgl::game::current_context();

    bd->prg = ctx->program({ s_vertex_shader, s_fragment_shader });
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
    io.BackendRendererName = "mgl::game::imgui";
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

    io.KeyMap[ImGuiKey_Tab] = static_cast<int>(key::Tab);
    io.KeyMap[ImGuiKey_LeftArrow] = static_cast<int>(key::Left);
    io.KeyMap[ImGuiKey_RightArrow] = static_cast<int>(key::Right);
    io.KeyMap[ImGuiKey_UpArrow] = static_cast<int>(key::Up);
    io.KeyMap[ImGuiKey_DownArrow] = static_cast<int>(key::Down);
    io.KeyMap[ImGuiKey_PageUp] = static_cast<int>(key::PageUp);
    io.KeyMap[ImGuiKey_PageDown] = static_cast<int>(key::PageDown);
    io.KeyMap[ImGuiKey_Home] = static_cast<int>(key::Home);
    io.KeyMap[ImGuiKey_End] = static_cast<int>(key::End);
    io.KeyMap[ImGuiKey_Insert] = static_cast<int>(key::Insert);
    io.KeyMap[ImGuiKey_Delete] = static_cast<int>(key::Delete);
    io.KeyMap[ImGuiKey_Backspace] = static_cast<int>(key::Backspace);
    io.KeyMap[ImGuiKey_Space] = static_cast<int>(key::Space);
    io.KeyMap[ImGuiKey_Enter] = static_cast<int>(key::Return);
    io.KeyMap[ImGuiKey_Escape] = static_cast<int>(key::Esc);
    io.KeyMap[ImGuiKey_KeyPadEnter] = static_cast<int>(key::NumPadEnter);
    io.KeyMap[ImGuiKey_A] = static_cast<int>(key::KeyA);
    io.KeyMap[ImGuiKey_B] = static_cast<int>(key::KeyB);
    io.KeyMap[ImGuiKey_C] = static_cast<int>(key::KeyC);
    io.KeyMap[ImGuiKey_D] = static_cast<int>(key::KeyD);
    io.KeyMap[ImGuiKey_E] = static_cast<int>(key::KeyE);
    io.KeyMap[ImGuiKey_F] = static_cast<int>(key::KeyF);
    io.KeyMap[ImGuiKey_G] = static_cast<int>(key::KeyG);
    io.KeyMap[ImGuiKey_H] = static_cast<int>(key::KeyH);
    io.KeyMap[ImGuiKey_I] = static_cast<int>(key::KeyI);
    io.KeyMap[ImGuiKey_J] = static_cast<int>(key::KeyJ);
    io.KeyMap[ImGuiKey_K] = static_cast<int>(key::KeyK);
    io.KeyMap[ImGuiKey_L] = static_cast<int>(key::KeyL);
    io.KeyMap[ImGuiKey_M] = static_cast<int>(key::KeyM);
    io.KeyMap[ImGuiKey_N] = static_cast<int>(key::KeyN);
    io.KeyMap[ImGuiKey_O] = static_cast<int>(key::KeyO);
    io.KeyMap[ImGuiKey_P] = static_cast<int>(key::KeyP);
    io.KeyMap[ImGuiKey_Q] = static_cast<int>(key::KeyQ);
    io.KeyMap[ImGuiKey_R] = static_cast<int>(key::KeyR);
    io.KeyMap[ImGuiKey_S] = static_cast<int>(key::KeyS);
    io.KeyMap[ImGuiKey_T] = static_cast<int>(key::KeyT);
    io.KeyMap[ImGuiKey_U] = static_cast<int>(key::KeyU);
    io.KeyMap[ImGuiKey_V] = static_cast<int>(key::KeyV);
    io.KeyMap[ImGuiKey_W] = static_cast<int>(key::KeyW);
    io.KeyMap[ImGuiKey_X] = static_cast<int>(key::KeyX);
    io.KeyMap[ImGuiKey_Y] = static_cast<int>(key::KeyY);
    io.KeyMap[ImGuiKey_Z] = static_cast<int>(key::KeyZ);
    io.KeyMap[ImGuiKey_LeftShift] = static_cast<int>(key::LeftShift);
    io.KeyMap[ImGuiKey_RightShift] = static_cast<int>(key::RightShift);
    io.KeyMap[ImGuiKey_LeftCtrl] = static_cast<int>(key::LeftCtrl);
    io.KeyMap[ImGuiKey_RightCtrl] = static_cast<int>(key::RightCtrl);
    io.KeyMap[ImGuiKey_LeftAlt] = static_cast<int>(key::LeftAlt);
    io.KeyMap[ImGuiKey_RightAlt] = static_cast<int>(key::RightAlt);
    io.KeyMap[ImGuiKey_0] = static_cast<int>(key::Key0);
    io.KeyMap[ImGuiKey_1] = static_cast<int>(key::Key1);
    io.KeyMap[ImGuiKey_2] = static_cast<int>(key::Key2);
    io.KeyMap[ImGuiKey_3] = static_cast<int>(key::Key3);
    io.KeyMap[ImGuiKey_4] = static_cast<int>(key::Key4);
    io.KeyMap[ImGuiKey_5] = static_cast<int>(key::Key5);
    io.KeyMap[ImGuiKey_6] = static_cast<int>(key::Key6);
    io.KeyMap[ImGuiKey_7] = static_cast<int>(key::Key7);
    io.KeyMap[ImGuiKey_8] = static_cast<int>(key::Key8);
    io.KeyMap[ImGuiKey_9] = static_cast<int>(key::Key9);
    io.KeyMap[ImGuiKey_F1] = static_cast<int>(key::F1);
    io.KeyMap[ImGuiKey_F2] = static_cast<int>(key::F2);
    io.KeyMap[ImGuiKey_F3] = static_cast<int>(key::F3);
    io.KeyMap[ImGuiKey_F4] = static_cast<int>(key::F4);
    io.KeyMap[ImGuiKey_F5] = static_cast<int>(key::F5);
    io.KeyMap[ImGuiKey_F6] = static_cast<int>(key::F6);
    io.KeyMap[ImGuiKey_F7] = static_cast<int>(key::F7);
    io.KeyMap[ImGuiKey_F8] = static_cast<int>(key::F8);
    io.KeyMap[ImGuiKey_F9] = static_cast<int>(key::F9);
    io.KeyMap[ImGuiKey_F10] = static_cast<int>(key::F10);
    io.KeyMap[ImGuiKey_F11] = static_cast<int>(key::F11);
    io.KeyMap[ImGuiKey_F12] = static_cast<int>(key::F12);

    io.DisplaySize =
        ImVec2(mgl::game::current_window().width(), mgl::game::current_window().height());
    io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
  }

  void shutdown()
  {
    MGL_CORE_ASSERT(ImGui::GetCurrentContext() != nullptr, "ImGui Context not initialized");

    ImGuiIO& io = ImGui::GetIO();
    MGL_CORE_ASSERT(io.BackendRendererUserData != nullptr, "Backend not initialized");

    auto bd = static_cast<imgui_render_data*>(io.BackendRendererUserData);

    mgl::game::current_context()->enter();
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

  void on_event(mgl::game::event& event)
  {
    if(!is_initialized())
      return;

    EventDispatcher dispatcher(event);

    // Dispatch Windows Events
    dispatcher.dispatch<window_close_event>(MGL_BIND_EVENT_FN(on_window_close));
    dispatcher.dispatch<window_resize_event>(MGL_BIND_EVENT_FN(on_window_resize));

    // Dispatch key events to be handled by the application
    dispatcher.dispatch<key_pressed_event>(MGL_BIND_EVENT_FN(on_key_pressed));
    dispatcher.dispatch<key_released_event>(MGL_BIND_EVENT_FN(on_key_released));

    // Dispatch mouse events to be handled by the application
    dispatcher.dispatch<mouse_moved_event>(MGL_BIND_EVENT_FN(on_mouse_moved));
    dispatcher.dispatch<mouse_scrolled_event>(MGL_BIND_EVENT_FN(on_mouse_scrolled));
    dispatcher.dispatch<mouse_button_pressed_event>(MGL_BIND_EVENT_FN(on_mouse_button_pressed));
    dispatcher.dispatch<mouse_button_released_event>(MGL_BIND_EVENT_FN(on_mouse_button_released));
  }

  bool is_initialized()
  {
    return ImGui::GetCurrentContext() != nullptr;
  }

  void render()
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

    auto ctx = mgl::game::current_context();
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

  bool on_window_close(window_close_event& event)
  {
    return false;
  }

  bool on_window_resize(window_resize_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    MGL_CORE_ASSERT(io.BackendRendererUserData != nullptr, "Not initialized");
    const auto& e = static_cast<const mgl::game::window_resize_event&>(event);
    io.DisplaySize = ImVec2(e.width(), e.height());
    return false;
  }

  bool on_key_pressed(key_pressed_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    MGL_CORE_ASSERT(io.BackendRendererUserData != nullptr, "Not initialized");
    const auto& e = static_cast<const mgl::game::key_pressed_event&>(event);
    io.KeysDown[static_cast<int>(e.key())] = true;
    return false;
  }

  bool on_key_released(key_released_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    MGL_CORE_ASSERT(io.BackendRendererUserData != nullptr, "Not initialized");
    const auto& e = static_cast<const mgl::game::key_released_event&>(event);
    io.KeysDown[static_cast<int>(e.key())] = false;
    return false;
  }

  bool on_mouse_moved(mouse_moved_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    MGL_CORE_ASSERT(io.BackendRendererUserData != nullptr, "Not initialized");
    const auto& e = static_cast<const mgl::game::mouse_moved_event&>(event);
    io.MousePos = ImVec2(e.x(), e.y());
    return false;
  }

  bool on_mouse_scrolled(mouse_scrolled_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    MGL_CORE_ASSERT(io.BackendRendererUserData != nullptr, "Not initialized");
    const auto& e = static_cast<const mgl::game::mouse_scrolled_event&>(event);
    io.MouseWheelH += e.x_offset();
    io.MouseWheel += e.y_offset();
    return false;
  }

  bool on_mouse_button_pressed(mouse_button_pressed_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    MGL_CORE_ASSERT(io.BackendRendererUserData != nullptr, "Not initialized");
    const auto& e = static_cast<const mgl::game::mouse_button_pressed_event&>(event);
    io.MouseDown[static_cast<int>(e.button())] = true;
    return false;
  }

  bool on_mouse_button_released(mouse_button_released_event& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    MGL_CORE_ASSERT(io.BackendRendererUserData != nullptr, "Not initialized");
    const auto& e = static_cast<const mgl::game::mouse_button_released_event&>(event);
    io.MouseDown[static_cast<int>(e.button())] = false;
    return false;
  }

} // namespace mgl::game::imgui