#pragma once

#include "mgl_core/memory.hpp"
namespace mgl::opengl
{
  class context;
  using context_ref = mgl::ref<context>;

  class attachment
  {
public:
    enum type
    {
      TEXTURE,
      RENDERBUFFER
    };

    virtual ~attachment() = default;
    virtual attachment::type attachment_type() = 0;

    virtual int32_t width() const = 0;

    virtual int32_t height() const = 0;

    virtual int32_t components() const = 0;

    virtual int32_t samples() const = 0;

    virtual bool depth() const = 0;

    virtual int32_t glo() const = 0;

    virtual const context_ref& ctx() const = 0;
  };

  using attachment_ref = mgl::ref<attachment>;
  using attachments_ref = mgl::ref_list<attachment>;

} // namespace  mgl::opengl