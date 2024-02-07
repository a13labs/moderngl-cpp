#pragma once
#include "attachment.hpp"
#include "data_type.hpp"
#include "gl_object.hpp"

#include "mgl_core/memory.hpp"

namespace mgl::opengl
{
  class renderbuffer : public attachment, public gl_object
  {
public:
    ~renderbuffer() = default;

    virtual void release() override final;

    virtual attachment::type attachment_type() override { return attachment::type::RENDERBUFFER; }

    virtual int32_t width() const override { return m_width; }

    virtual int32_t height() const override { return m_height; }

    virtual int32_t samples() const override { return m_samples; }

    virtual bool depth() const override { return m_depth; }

    virtual int32_t components() const override { return m_components; }

    virtual int32_t glo() const override final { return gl_object::glo(); }

    virtual context_ref& ctx() override final { return gl_object::ctx(); }

private:
    friend class context;

    renderbuffer(const context_ref& ctx,
                 int32_t w,
                 int32_t h,
                 int32_t components,
                 int32_t samples,
                 const std::string& dtype);

    renderbuffer(const context_ref& ctx, int32_t w, int32_t h, int32_t samples);

    data_type* m_data_type;
    int32_t m_width;
    int32_t m_height;
    int32_t m_components;
    int32_t m_samples;
    bool m_depth;
  };

  using renderbuffer_ref = mgl::ref<mgl::opengl::renderbuffer>;

} // namespace  mgl::opengl