/*
   Copyright 2022 Alexandre Pires (c.alexandre.pires@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#pragma once

#include "input.hpp"

#include "mgl_core/string.hpp"
#include "mgl_core/utils.hpp"

#include <functional>

#define MGL_CLS_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#define MGL_BIND_EVENT_FN(fn) std::bind(&fn, std::placeholders::_1)

namespace mgl::game
{

  enum class EventType
  {
    None = 0,
    WindowClose,
    WindowResize,
    WindowFocus,
    WindowLostFocus,
    WindowMoved,
    AppTick,
    AppUpdate,
    AppRender,
    KeyPressed,
    KeyReleased,
    KeyTyped,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled,
    GamepadButtonPressed,
    GamepadButtonReleased,
    GamepadAxisMoved
  };

  enum EventCategory
  {
    None = 0,
    EventCategoryApplication = BIT(0),
    EventCategoryInput = BIT(1),
    EventCategoryKeyboard = BIT(2),
    EventCategoryMouse = BIT(3),
    EventCategoryMouseButton = BIT(4),
    EventCategoryGamepad = BIT(5),
    EventCategoryGamepadButton = BIT(6)
  };

#define EVENT_CLASS_TYPE(type)                                                                     \
  static EventType get_static_type()                                                               \
  {                                                                                                \
    return EventType::type;                                                                        \
  }                                                                                                \
  virtual EventType get_event_type() const override                                                \
  {                                                                                                \
    return get_static_type();                                                                      \
  }                                                                                                \
  virtual const char* get_name() const override                                                    \
  {                                                                                                \
    return #type;                                                                                  \
  }

#define EVENT_CLASS_CATEGORY(category)                                                             \
  virtual int get_category_flags() const override                                                  \
  {                                                                                                \
    return category;                                                                               \
  }

  class event
  {
public:
    virtual EventType get_event_type() const = 0;
    virtual const char* get_name() const = 0;
    virtual int get_category_flags() const = 0;
    virtual std::string to_string() const { return get_name(); }

    bool is_category(EventCategory category) { return get_category_flags() & category; }

    inline bool is_handled() const { return mHandled; }

    inline void set_handled(bool handled) { mHandled = handled; }

protected:
    bool mHandled = false;
  };

  using event_handler = std::function<void(event&)>;

  class EventDispatcher
  {
public:
    EventDispatcher(event& event)
        : m_event(event)
    { }

    // F will be deduced by the compiler
    template <typename T, typename F>
    bool dispatch(const F& func)
    {
      if(m_event.get_event_type() == T::get_static_type())
      {
        m_event.set_handled(func(static_cast<T&>(m_event)));
        return true;
      }
      return false;
    }

private:
    event& m_event;
  };

  inline std::ostream& operator<<(std::ostream& os, const event& e)
  {
    return os << e.to_string();
  }

  class window_resize_event : public event
  {
public:
    window_resize_event(uint32_t width, uint32_t height)
        : m_width(width)
        , m_height(height)
    { }

    uint32_t width() const { return m_width; }
    uint32_t height() const { return m_height; }

    std::string to_string() const override
    {
      std::stringstream ss;
      ss << "WindowResizeEvent: " << m_width << ", " << m_height;
      return ss.str();
    }

    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
private:
    uint32_t m_width, m_height;
  };

  class window_close_event : public event
  {
public:
    window_close_event() = default;

    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
  };

  class mouse_moved_event : public event
  {
public:
    mouse_moved_event(float x, float y)
        : m_mouse_x(x)
        , m_mouse_y(y)
    { }

    float x() const { return m_mouse_x; }
    float y() const { return m_mouse_y; }

    std::string to_string() const override
    {
      std::stringstream ss;
      ss << "MouseMovedEvent: " << m_mouse_x << ", " << m_mouse_y;
      return ss.str();
    }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
    float m_mouse_x, m_mouse_y;
  };

  class mouse_scrolled_event : public event
  {
public:
    mouse_scrolled_event(float xOffset, float yOffset)
        : m_x_offset(xOffset)
        , m_y_offset(yOffset)
    { }

    float x_offset() const { return m_x_offset; }
    float y_offset() const { return m_y_offset; }

    std::string to_string() const override
    {
      std::stringstream ss;
      ss << "MouseScrolledEvent: " << x_offset() << ", " << y_offset();
      return ss.str();
    }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
    float m_x_offset, m_y_offset;
  };

  class MouseButtonEvent : public event
  {
public:
    inline mouse_button::name button() const { return m_button; }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryMouseButton | EventCategoryInput)
protected:
    MouseButtonEvent(mouse_button::name button)
        : m_button(button)
    { }

    mouse_button::name m_button;
  };

  class mouse_button_pressed_event : public MouseButtonEvent
  {
public:
    mouse_button_pressed_event(mouse_button::name button)
        : MouseButtonEvent(button)
    { }

    std::string to_string() const override
    {
      std::stringstream ss;
      ss << "MouseButtonPressedEvent: " << m_button;
      return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonPressed)
  };

  class mouse_button_released_event : public MouseButtonEvent
  {
public:
    mouse_button_released_event(mouse_button::name button)
        : MouseButtonEvent(button)
    { }

    std::string to_string() const override
    {
      std::stringstream ss;
      ss << "MouseButtonReleasedEvent: " << m_button;
      return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonReleased)
  };

  class KeyEvent : public event
  {
public:
    key::name key() const { return m_key_code; }
    uint8_t modifiers() const { return m_modifiers; }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
    KeyEvent(key::name keycode, uint8_t modifiers)
        : m_key_code(keycode)
        , m_modifiers(modifiers)
    { }

    key::name m_key_code;
    uint8_t m_modifiers;
  };

  class key_pressed_event : public KeyEvent
  {
public:
    key_pressed_event(key::name keycode, uint8_t modifiers, int repeat)
        : KeyEvent(keycode, modifiers)
        , m_repeat(repeat)
    { }

    int repeat_count() const { return m_repeat; }

    std::string to_string() const override
    {
      std::stringstream ss;
      ss << "KeyPressedEvent: " << m_key_code << " ( repeat:" << m_repeat << ")";
      return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPressed)
private:
    bool m_repeat;
  };

  class key_released_event : public KeyEvent
  {
public:
    key_released_event(key::name keycode, uint8_t modifiers)
        : KeyEvent(keycode, modifiers)
    { }

    std::string to_string() const override
    {
      std::stringstream ss;
      ss << "KeyReleasedEvent: " << m_key_code;
      return ss.str();
    }

    EVENT_CLASS_TYPE(KeyReleased)
  };

  class key_typed_event : public KeyEvent
  {
public:
    key_typed_event(key::name keycode, uint8_t modifiers)
        : KeyEvent(keycode, modifiers)
    { }

    std::string to_string() const override
    {
      std::stringstream ss;
      ss << "KeyTypedEvent: " << m_key_code;
      return ss.str();
    }

    EVENT_CLASS_TYPE(KeyTyped)
  };
} // namespace  mgl::game
