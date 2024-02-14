#pragma once
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
  class texture_3d : public texture, public gl_object
  {
public:
    ~texture_3d() = default;

    virtual void release() override;

    virtual texture::type texture_type() override { return texture::type::TEXTURE_3D; }

    virtual int32_t width() const override { return m_width; }

    virtual int32_t height() const override { return m_height; }

    virtual int32_t components() const override { return m_components; }

    virtual void use(int32_t index = 0) override;

    virtual int32_t glo() const override { return gl_object::glo(); }

    virtual const context_ref& ctx() const override { return gl_object::ctx(); }

    int32_t depth() const { return m_depth; }

    bool repeat_x() const { return m_repeat_x; }

    void set_repeat_x(bool value);

    bool repeat_y() const { return m_repeat_y; }

    void set_repeat_y(bool value);

    bool repeat_z() const { return m_repeat_z; }

    void set_repeat_z(bool value);

    const texture::filter& filter() const { return m_filter; }

    void set_filter(const texture::filter& value);

    std::string swizzle();

    void set_swizzle(const std::string& value);

    void read(mgl::uint8_buffer& dst, int32_t align = 1, size_t dst_offset = 0);

    void read(buffer_ref& dst, int32_t align = 1, size_t dst_offset = 0);

    void write(const mgl::uint8_buffer& src, const mgl::cube& v, int32_t align = 1);

    void write(const mgl::uint8_buffer& src, int32_t align = 1);

    void write(const buffer_ref& src, const mgl::cube& v, int32_t align = 1);

    void write(const buffer_ref& src, int32_t align = 1);

    void bind_to_image(
        int32_t unit, bool read = true, bool write = true, int32_t lvl = 0, int32_t format = 0);

    void build_mipmaps(int32_t base = 0, int32_t max_lvl = 1000);

private:
    friend class context;

    texture_3d(const context_ref& ctx,
               int32_t w,
               int32_t h,
               int32_t depth,
               int32_t components,
               const void* data,
               int32_t align,
               const std::string& dtype);

    data_type* m_data_type;
    int32_t m_width;
    int32_t m_height;
    int32_t m_depth;
    int32_t m_components;
    texture::filter m_filter;
    int32_t m_max_lvl;
    bool m_repeat_x;
    bool m_repeat_y;
    bool m_repeat_z;
  };

  using texture_3d_ref = mgl::ref<texture_3d>;

} // namespace  mgl::opengl