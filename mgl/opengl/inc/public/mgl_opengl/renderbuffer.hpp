#pragma once
#include "attachment.hpp"
#include "data_type.hpp"

#include "mgl_core/memory.hpp"

namespace mgl::opengl
{
  class context;

  class renderbuffer : public attachment
  {
public:
    renderbuffer(
        int32_t w, int32_t h, int32_t components, int32_t samples, const std::string& dtype);

    renderbuffer(int32_t w, int32_t h, int32_t samples);

    ~renderbuffer() = default;

    void release();
    bool released() const { return m_glo == 0; };

    virtual attachment::type attachment_type() override { return attachment::type::RENDERBUFFER; }
    int32_t width() const { return m_width; }
    int32_t height() const { return m_height; }
    int32_t samples() const { return m_samples; }
    bool depth() const { return m_depth; }
    int32_t components() const { return m_components; }

    int32_t glo() const { return m_glo; }

private:
    data_type* m_data_type;
    int32_t m_glo;
    int32_t m_width;
    int32_t m_height;
    int32_t m_components;
    int32_t m_samples;
    bool m_depth;
  };

  using renderbuffer_ref = mgl::ref<mgl::opengl::renderbuffer>;

} // namespace  mgl::opengl