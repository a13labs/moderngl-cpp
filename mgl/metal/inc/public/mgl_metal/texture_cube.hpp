#pragma once

#include "attachment.hpp"
#include "buffer.hpp"
#include "enums.hpp"
#include "object.hpp"
#include "texture.hpp"

#include "mgl_core/math.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"

namespace mgl::metal
{
  class texture_cube : public attachment, public texture
  {
public:
    enum face
    {
      POSITIVE_X = 0,
      NEGATIVE_X = 1,
      POSITIVE_Y = 2,
      NEGATIVE_Y = 3,
      POSITIVE_Z = 4,
      NEGATIVE_Z = 5,
    };

    ~texture_cube() = default;

    virtual void release() override;

    virtual attachment::type attachment_type() override { return attachment::type::TEXTURE; }

    virtual texture::type texture_type() override { return texture::type::TEXTURE_CUBE; }

    virtual int32_t width() const override { return m_size; }

    virtual int32_t height() const override { return m_size; }

    virtual int32_t samples() const override { return m_samples; }

    virtual bool depth() const override { return m_depth; }

    virtual int32_t components() const override { return m_components; }

    virtual const context_ref& ctx() const override { return m_ctx; }

    int32_t size() const { return m_size; }

    const texture::filter& filter() const { return m_filter; }

    void set_filter(const texture::filter& value);

    mgl::metal::compare_func compare_func() const { return m_compare_func; }

    void set_compare_func(mgl::metal::compare_func value);

    float anisotropy() const { return m_anisotropy; }

    void set_anisotropy(float value);

    std::string swizzle() const;

    void set_swizzle(const std::string& value);

    void
    read(face f, mgl::uint8_buffer& dst, int32_t lvl = 0, int32_t align = 1, size_t dst_off = 0);

    void read(face f, buffer_ref& dst, int32_t lvl = 0, int32_t align = 1, size_t dst_off = 0);

    void write(face f,
               const mgl::uint8_buffer& src,
               const mgl::rect& v,
               int32_t lvl = 0,
               int32_t align = 1);

    void write(face f, const mgl::uint8_buffer& src, int32_t lvl = 0, int32_t align = 1);

    void
    write(face f, const buffer_ref& src, const mgl::rect& v, int32_t lvl = 0, int32_t align = 1);

    void write(face f, const buffer_ref& src, int32_t lvl = 0, int32_t align = 1);

    void bind_to_image(
        int32_t unit, bool read = true, bool write = true, int32_t lvl = 0, int32_t f = 0);

    void build_mipmaps(int32_t base = 0, int32_t max_lvl = 1000);

    virtual void use(int32_t index = 0) override;

    // Metal-specific methods
    void* native_texture() const { return m_native_texture; }

private:
    friend class context;

    texture_cube(const context_ref& ctx,
                 int32_t size,
                 int32_t components,
                 const void* data,
                 int32_t samples,
                 int32_t align,
                 const std::string& dtype);

    texture_cube(
        const context_ref& ctx, int32_t size, const void* data, int32_t samples, int32_t align);

    context_ref m_ctx;
    void* m_native_texture;
    int32_t m_size;
    bool m_depth;
    int32_t m_samples;
    int32_t m_components;
    texture::filter m_filter;
    int32_t m_max_lvl;
    mgl::metal::compare_func m_compare_func;
    float m_anisotropy;
  };

  using texture_cube_ref = mgl::ref<texture_cube>;

} // namespace mgl::metal