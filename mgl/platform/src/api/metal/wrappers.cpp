#include "mgl_platform/api/metal/program.hpp"
#include "mgl_platform/api/metal/textures.hpp"
#include "mgl_platform/api/metal/api.hpp"

namespace mgl::platform::api::backends
{
  metal_program::metal_program(const std::string& vs_source,
                               const std::string& fs_source,
                               const std::string& gs_source,
                               const std::string& tes_source,
                               const std::string& tcs_source,
                               const std::string& filename)
  {
    // Create native program using the current Metal context
  auto ctx = mgl::platform::api::backends::metal_api::current_context();
    if(ctx)
    {
      m_program = ctx->program({ vs_source, fs_source, gs_source }, {}, {}, true, filename);
    }
  }

  metal_texture_2d::metal_texture_2d(const mgl::size& size, int32_t components, int32_t samples)
  {
  auto ctx = mgl::platform::api::backends::metal_api::current_context();
    if(ctx)
    {
      m_texture = ctx->texture2d(size.width, size.height, components, nullptr, samples);
    }
  }

  const texture::filter& metal_texture_2d::get_filter() const
  {
    return m_filter;
  }

  void metal_texture_2d::set_filter(const texture::filter& value)
  {
    m_filter = value;
    if(m_texture)
    {
      // Convert platform filter enum to metal texture filter struct (placeholder mapping)
      mgl::metal::texture::filter f{ static_cast<int>(value), static_cast<int>(value) };
      m_texture->set_filter(f);
    }
  }

} // namespace mgl::platform::api::backends
