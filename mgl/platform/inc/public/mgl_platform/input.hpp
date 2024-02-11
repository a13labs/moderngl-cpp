#pragma once

namespace mgl::platform
{
  struct key
  {
    enum name
    {
      None = 0,
      Esc,
      Return,
      Tab,
      Space,
      Backspace,
      Up,
      Down,
      Left,
      Right,
      Insert,
      Delete,
      Home,
      End,
      PageUp,
      PageDown,
      Print,
      Plus,
      Minus,
      LeftBracket,
      RightBracket,
      Semicolon,
      Quote,
      Comma,
      Period,
      Slash,
      Backslash,
      Tilde,
      F1,
      F2,
      F3,
      F4,
      F5,
      F6,
      F7,
      F8,
      F9,
      F10,
      F11,
      F12,
      NumPad0,
      NumPad1,
      NumPad2,
      NumPad3,
      NumPad4,
      NumPad5,
      NumPad6,
      NumPad7,
      NumPad8,
      NumPad9,
      NumPadEnter,
      Key0,
      Key1,
      Key2,
      Key3,
      Key4,
      Key5,
      Key6,
      Key7,
      Key8,
      Key9,
      KeyA,
      KeyB,
      KeyC,
      KeyD,
      KeyE,
      KeyF,
      KeyG,
      KeyH,
      KeyI,
      KeyJ,
      KeyK,
      KeyL,
      KeyM,
      KeyN,
      KeyO,
      KeyP,
      KeyQ,
      KeyR,
      KeyS,
      KeyT,
      KeyU,
      KeyV,
      KeyW,
      KeyX,
      KeyY,
      KeyZ,

      LeftAlt,
      RightAlt,
      LeftCtrl,
      RightCtrl,
      LeftShift,
      RightShift,
      LeftMeta,
      RightMeta,

      Count
    };
  };

  struct mouse_button
  {
    enum name
    {
      Left,
      Middle,
      Right,
      None,

      Count
    };
  };

  struct gamepad_axis
  {
    enum name
    {
      LeftX,
      LeftY,
      LeftZ,
      RightX,
      RightY,
      RightZ,

      Count
    };
  };

  struct input_state
  {
    bool pressed_keys[key::Count];
    bool pressed_mouse_buttons[mouse_button::Count];
  };

  extern input_state s_input_state;

  inline bool is_key_pressed(key::name key)
  {
    return s_input_state.pressed_keys[key & 0xff];
  }

  inline bool IsMouseButtonPressed(mouse_button::name button)
  {
    return s_input_state.pressed_mouse_buttons[button & 0xff];
  }

} // namespace  mgl::platform
