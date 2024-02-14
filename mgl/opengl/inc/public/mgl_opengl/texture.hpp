#pragma once

namespace mgl::opengl
{
  class texture
  {
public:
    enum type
    {
      TEXTURE_2D,
      TEXTURE_3D,
      TEXTURE_CUBE,
    };

    struct filter
    {
      int min_filter;
      int mag_filter;
    };

    virtual ~texture() = default;

    virtual texture::type texture_type() = 0;

    virtual int32_t width() const = 0;

    virtual int32_t height() const = 0;

    virtual int32_t components() const = 0;

    virtual void use(int index) = 0;

    virtual void release() = 0;

    virtual int32_t glo() const = 0;

    virtual const context_ref& ctx() const = 0;
  };

  using texture_ref = mgl::ref<mgl::opengl::texture>;
  using textures = mgl::ref_list<texture>;

} // namespace  mgl::opengl