#pragma once
#include "attachment.hpp"
#include "buffer.hpp"
#include "data_type.hpp"
#include "enums.hpp"
#include "gl_object.hpp"
#include "texture.hpp"

#include "mgl_core/math.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"

namespace mgl::opengl
{
  class texture_2d : public attachment, public texture, public gl_object
  {
public:
    ~texture_2d() = default;

    virtual void release() override;

    virtual attachment::type attachment_type() override { return attachment::type::TEXTURE; }

    virtual texture::type texture_type() override { return texture::type::TEXTURE_2D; }

    virtual int32_t width() const override { return m_width; }

    virtual int32_t height() const override { return m_height; }

    virtual int32_t samples() const override { return m_samples; }

    virtual bool depth() const override { return m_depth; }

    virtual int32_t components() const override { return m_components; }

    virtual int32_t glo() const override { return gl_object::glo(); }

    virtual const context_ref& ctx() const override { return gl_object::ctx(); }

    bool repeat_x() const { return m_repeat_x; }

    void set_repeat_x(bool value);

    bool repeat_y() const { return m_repeat_y; }

    void set_repeat_y(bool value);

    const texture::filter& filter() const { return m_filter; }

    void set_filter(const texture::filter& value);

    mgl::opengl::compare_func compare_func() const { return m_compare_func; }

    void set_compare_func(mgl::opengl::compare_func value);

    float anisotropy() const { return m_anisotropy; }

    void set_anisotropy(float value);

    std::string swizzle() const;

    void set_swizzle(const std::string& value);

    void read(mgl::uint8_buffer& dst, int32_t lvl = 0, int32_t align = 1, size_t dst_off = 0);

    void read(buffer_ref& dst, int32_t lvl = 0, int32_t align = 1, size_t dst_off = 0);

    void
    write(const mgl::uint8_buffer& src, const mgl::rect& v, int32_t lvl = 0, int32_t align = 1);

    void write(const mgl::uint8_buffer& src, int32_t lvl = 0, int32_t align = 1);

    void write(const buffer_ref& src, const mgl::rect& v, int32_t lvl = 0, int32_t align = 1);

    void write(const buffer_ref& src, int32_t lvl = 0, int32_t align = 1);

    void resize(int32_t w, int32_t h, int32_t components = 4, const mgl::uint8_buffer& data = {});

    void bind_to_image(
        int32_t unit, bool read = true, bool write = true, int32_t lvl = 0, int32_t f = 0);

    void build_mipmaps(int32_t base = 0, int32_t max_lvl = 1000);

    virtual void use(int32_t index = 0) override;

private:
    friend class context;

    texture_2d(const context_ref& ctx,
               int32_t w,
               int32_t h,
               int32_t components,
               const void* data,
               int32_t samples,
               int32_t align,
               const std::string& dtype,
               int32_t internal_format_override);

    texture_2d(const context_ref& ctx,
               int32_t w,
               int32_t h,
               const void* data,
               int32_t samples,
               int32_t align);

    data_type* m_data_type;
    int32_t m_width;
    int32_t m_height;
    bool m_depth;
    int32_t m_samples;
    int32_t m_components;
    texture::filter m_filter;
    int32_t m_max_lvl;
    mgl::opengl::compare_func m_compare_func;
    float m_anisotropy;
    bool m_repeat_x;
    bool m_repeat_y;
  };

  using texture_2d_ref = mgl::ref<texture_2d>;

} // namespace  mgl::opengl