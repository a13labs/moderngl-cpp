#include "mgl_registry/resources/image.hpp"
#include "mgl_core/debug.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION 1
#include "stb/stb_image_write.h"

namespace mgl::registry
{
  image::image(uint32_t width, uint32_t height, uint32_t channels)
      : m_width(width)
      , m_height(height)
      , m_channels(channels)
      , m_data(width * height * channels)
  {
    std::fill(m_data.begin(), m_data.end(), 0);
  }

  image::image(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data)
      : m_width(width)
      , m_height(height)
      , m_channels(channels)
      , m_data(width * height * channels)
  {
    MGL_CORE_ASSERT(data, "Data is null");
    std::copy(data, data + width * height * channels, m_data.begin());
  }

  void image::blit(uint32_t x, uint32_t y, const image& image)
  {
    MGL_CORE_ASSERT(x >= 0 && x < m_width, "X is out of bounds");
    MGL_CORE_ASSERT(y >= 0 && y < m_height, "Y is out of bounds");
    MGL_CORE_ASSERT(x + image.width() <= m_width, "Image is out of bounds");
    MGL_CORE_ASSERT(y + image.height() <= m_height, "Image is out of bounds");

    for(uint32_t i = 0; i < image.height(); ++i)
    {
      for(uint32_t j = 0; j < image.width(); ++j)
      {
        glm::vec4 color = image.get_pixel(j, i);
        put_pixel(x + j, y + i, color);
      }
    }
  }

  image_ref image::crop(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const
  {
    MGL_CORE_ASSERT(x >= 0 && x < m_width, "X is out of bounds");
    MGL_CORE_ASSERT(y >= 0 && y < m_height, "Y is out of bounds");
    MGL_CORE_ASSERT(x + width <= m_width, "Width is out of bounds");
    MGL_CORE_ASSERT(y + height <= m_height, "Height is out of bounds");

    uint8_buffer data(width * height * m_channels);
    for(uint32_t i = 0; i < height; ++i)
    {
      for(uint32_t j = 0; j < width; ++j)
      {
        for(uint32_t k = 0; k < m_channels; ++k)
        {
          data[i * width * m_channels + j * m_channels + k] =
              m_data[(y + i) * m_width * m_channels + (x + j) * m_channels + k];
        }
      }
    }

    return mgl::create_ref<image>(width, height, m_channels, data);
  }

  void image::put_pixel(uint32_t x, uint32_t y, const glm::vec4& color)
  {
    MGL_CORE_ASSERT(x >= 0 && x < m_width, "X is out of bounds");
    MGL_CORE_ASSERT(y >= 0 && y < m_height, "Y is out of bounds");

    for(uint32_t i = 0; i < m_channels; ++i)
    {
      m_data[y * m_width * m_channels + x * m_channels + i] = color[i] * 255;
    }
  }

  glm::vec4 image::get_pixel(uint32_t x, uint32_t y) const
  {
    MGL_CORE_ASSERT(x >= 0 && x < m_width, "X is out of bounds");
    MGL_CORE_ASSERT(y >= 0 && y < m_height, "Y is out of bounds");

    glm::vec4 color;
    for(uint32_t i = 0; i < m_channels; ++i)
    {
      color[i] = m_data[y * m_width * m_channels + x * m_channels + i] / 255.0f;
    }

    return color;
  }

  void image::fill(const glm::vec4& color)
  {
    for(uint32_t i = 0; i < m_height; ++i)
    {
      for(uint32_t j = 0; j < m_width; ++j)
      {
        for(uint32_t k = 0; k < m_channels; ++k)
        {
          m_data[i * m_width * m_channels + j * m_channels + k] = color[k] * 255;
        }
      }
    }
  }

  void image::save(const location_ref& location, const std::string& path) const
  {
    auto file = location->open_write(path);
    if(!file)
    {
      MGL_CORE_ERROR("Failed to open file for writing: {0}", path);
      return;
    }

    stbi_write_png_to_func(
        [](void* context, void* data, int size) {
          auto file = static_cast<mgl::io::ostream*>(context);
          file->write((char*)data, size);
        },
        file.get(),
        m_width,
        m_height,
        m_channels,
        m_data.data(),
        0);
  }

} // namespace mgl::registry