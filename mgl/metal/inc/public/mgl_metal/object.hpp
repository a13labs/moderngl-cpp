#pragma once

#include "mgl_core/memory.hpp"

namespace mgl::metal
{
  class context;
  using context_ref = mgl::ref<context>;

  using metal_handler = void*;
  class object
  {
public:
    object(const context_ref& ctx)
        : m_ctx(ctx)
    { }

    virtual ~object() = default;

    const context_ref& ctx() const { return m_ctx; }

    metal_handler handler() const { return m_handler; }

    /**
     * @brief Checks if the buffer has been released.
     * @return True if the buffer is released, false otherwise.
     */
     bool released() const { return m_handler == nullptr; }

    bool operator==(const object& other) const
    {
      return m_handler == other.m_handler && m_ctx == other.m_ctx;
    }

    bool operator!=(const object& other) const { return !(*this == other); }

    bool operator==(const object* other) const
    {
      return m_handler == other->m_handler && m_ctx == other->m_ctx;
    }

    bool operator!=(const object* other) const { return !(*this == other); }
    
    virtual void release() = 0;
protected:
    metal_handler m_handler{nullptr};

private:
    context_ref m_ctx;
  };

} // namespace mgl::metal
