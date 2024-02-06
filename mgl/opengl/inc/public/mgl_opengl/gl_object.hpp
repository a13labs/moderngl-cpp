#pragma once

#include "mgl_core/memory.hpp"

namespace mgl::opengl
{
  class context;

  class gl_object
  {
public:
    gl_object(context* ctx)
        : m_ctx(ctx)
        , m_glo(0)
    { }

    virtual ~gl_object() = default;

    bool released() const { return m_glo == 0; }

    int32_t glo() const { return m_glo; }

    context* ctx() { return m_ctx; }

    virtual void release() = 0;

protected:
    context* m_ctx;
    int32_t m_glo = 0;
  };
} // namespace mgl::opengl