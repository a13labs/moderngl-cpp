#include "mgl_registry/resources/image.hpp"
#include "mgl_core/debug.hpp"

namespace mgl::registry
{
  image::image(int width, int height, int channels, unsigned char* data)
      : m_width(width)
      , m_height(height)
      , m_channels(channels)
      , m_data(width * height * channels)
  {
    MGL_CORE_ASSERT(data, "Data is null");
    std::copy(data, data + width * height * channels, m_data.begin());
  }

} // namespace mgl::registry