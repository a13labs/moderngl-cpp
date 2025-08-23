#pragma once

#include "mgl_core/memory.hpp"

namespace mgl::opengl
{
  class context;
  using context_ref = mgl::ref<context>;

  class gl_object
  {
public:
    gl_object(const context_ref& ctx)
        : m_ctx(ctx)
        , m_glo(0)
    { }

    virtual ~gl_object() = default;

    bool released() const { return m_glo == 0; }

    int32_t glo() const { return m_glo; }

    const context_ref& ctx() const { return m_ctx; }

    bool operator==(const gl_object& other) const
    {
      return m_glo == other.m_glo && m_ctx == other.m_ctx;
    }

    bool operator!=(const gl_object& other) const { return !(*this == other); }

    bool operator==(const gl_object* other) const
    {
      return m_glo == other->m_glo && m_ctx == other->m_ctx;
    }

    bool operator!=(const gl_object* other) const { return !(*this == other); }

    virtual void release() = 0;

protected:
    void set_glo(int32_t glo) { m_glo = glo; }

private:
    context_ref m_ctx;
    int32_t m_glo;
  };
} // namespace mgl::opengl