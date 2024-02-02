#include "mgl_registry/resources/image.hpp"
#include "mgl_core/debug.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION 1
#include "stb/stb_image_write.h"

namespace mgl::registry
{
  image::image(int32_t width, int32_t height, int32_t channels)
      : m_width(width)
      , m_height(height)
      , m_channels(channels)
      , m_data(width * height * channels)
  {
    std::fill(m_data.begin(), m_data.end(), 0);
  }

  image::image(int32_t width, int32_t height, int32_t channels, unsigned char* data)
      : m_width(width)
      , m_height(height)
      , m_channels(channels)
      , m_data(width * height * channels)
  {
    MGL_CORE_ASSERT(data, "Data is null");
    std::copy(data, data + width * height * channels, m_data.begin());
  }

  void image::blit(int32_t x, int32_t y, const image& image, const mgl::rect& rect)
  {
    MGL_CORE_ASSERT(x >= 0 && x <= m_width, "X is out of bounds");
    MGL_CORE_ASSERT(y >= 0 && y <= m_height, "Y is out of bounds");
    MGL_CORE_ASSERT(m_channels >= 1 && m_channels <= 4, "Invalid number of channels");
    MGL_CORE_ASSERT(m_channels == image.m_channels, "Invalid number of channels, must be the same")

    int32_t width = std::min(static_cast<int32_t>(rect.width), m_width - x);
    int32_t height = std::min(static_cast<int32_t>(rect.height), m_height - y);

    for(int32_t i = 0; i < height; ++i)
    {
      for(int32_t j = 0; j < width; ++j)
      {
        std::copy(image.m_data.begin() + (rect.y + i) * image.m_width * m_channels +
                      (rect.x + j) * m_channels,
                  image.m_data.begin() + (rect.y + i) * image.m_width * m_channels +
                      (rect.x + j) * m_channels + m_channels,
                  m_data.begin() + (y + i) * m_width * m_channels + (x + j) * m_channels);
      }
    }
  }

  image_ref image::crop(int32_t x, int32_t y, int32_t width, int32_t height) const
  {
    MGL_CORE_ASSERT(x >= 0 && x < m_width, "X is out of bounds");
    MGL_CORE_ASSERT(y >= 0 && y < m_height, "Y is out of bounds");
    MGL_CORE_ASSERT(x + width <= m_width, "Width is out of bounds");
    MGL_CORE_ASSERT(y + height <= m_height, "Height is out of bounds");

    auto result = mgl::create_ref<image>(width, height, m_channels);

    for(int32_t i = 0; i < height; ++i)
    {
      for(int32_t j = 0; j < width; ++j)
      {
        std::copy(m_data.begin() + (y + i) * m_width * m_channels + (x + j) * m_channels,
                  m_data.begin() + (y + i) * m_width * m_channels + (x + j) * m_channels +
                      m_channels,
                  result->m_data.begin() + i * width * m_channels + j * m_channels);
      }
    }

    return result;
  }

  image_ref image::flip_vertically() const
  {
    auto result = mgl::create_ref<image>(m_width, m_height, m_channels);
    for(int32_t i = 0; i < m_height; ++i)
    {
      std::copy(m_data.begin() + i * m_width * m_channels,
                m_data.begin() + i * m_width * m_channels + m_width * m_channels,
                result->m_data.begin() + (m_height - i - 1) * m_width * m_channels);
    }

    return result;
  }

  image_ref image::flip_horizontally() const
  {
    auto result = mgl::create_ref<image>(m_width, m_height, m_channels);

    for(int32_t i = 0; i < m_height; ++i)
    {
      for(int32_t j = 0; j < m_width; ++j)
      {
        std::copy(m_data.begin() + i * m_width * m_channels + j * m_channels,
                  m_data.begin() + i * m_width * m_channels + j * m_channels + m_channels,
                  result->m_data.begin() + i * m_width * m_channels +
                      (m_width - j - 1) * m_channels);
      }
    }

    return result;
  }

  image_ref image::clone() const
  {
    auto result = mgl::create_ref<image>(m_width, m_height, m_channels);
    std::copy(m_data.begin(), m_data.end(), result->m_data.begin());
    return result;
  }

  void image::put_pixel(int32_t x, int32_t y, const glm::vec4& color)
  {
    MGL_CORE_ASSERT(x >= 0 && x <= m_width, "X is out of bounds");
    MGL_CORE_ASSERT(y >= 0 && y <= m_height, "Y is out of bounds");
    MGL_CORE_ASSERT(m_channels >= 1 && m_channels <= 4, "Invalid number of channels");
    uint8_t data[m_channels];
    for(int32_t i = 0; i < m_channels; ++i)
    {
      data[i] = color[i] * 255;
    }

    std::copy(data, data + m_channels, m_data.begin() + y * m_width * m_channels + x * m_channels);
  }

  void image::resize(int32_t width, int32_t height)
  {
    MGL_CORE_ASSERT(width > 0 && height > 0, "Invalid size");

    mgl::uint8_buffer new_buffer(width * height * m_channels);
    int32_t new_width = std::min(width, m_width);
    int32_t new_height = std::min(height, m_height);

    for(int32_t i = 0; i < new_height; ++i)
    {
      std::copy(m_data.begin() + i * m_width * m_channels,
                m_data.begin() + i * m_width * m_channels + new_width * m_channels,
                new_buffer.begin() + i * width * m_channels);
    }

    m_width = width;
    m_height = height;
    m_data = std::move(new_buffer);
  }

  glm::vec4 image::get_pixel(int32_t x, int32_t y) const
  {
    MGL_CORE_ASSERT(x >= 0 && x < m_width, "X is out of bounds");
    MGL_CORE_ASSERT(y >= 0 && y < m_height, "Y is out of bounds");

    glm::vec4 color;
    for(int32_t i = 0; i < m_channels; ++i)
    {
      color[i] = m_data[y * m_width * m_channels + x * m_channels + i] / 255.0f;
    }

    return color;
  }

  void image::fill(const glm::vec4& color)
  {
    MGL_CORE_ASSERT(m_channels >= 1 && m_channels <= 4, "Invalid number of channels");
    uint8_t data[m_channels];

    for(int32_t i = 0; i < m_channels; ++i)
    {
      data[i] = color[i] * 255;
    }

    for(int32_t i = 0; i < m_width * m_height; ++i)
    {
      std::copy(data, data + m_channels, m_data.begin() + i * m_channels);
    }
  }

  void image::save(const std::string& path, const location_ref& location) const
  {
    if(location)
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
    else
    {
      stbi_write_png(path.c_str(), m_width, m_height, m_channels, m_data.data(), 0);
    }
  }

} // namespace mgl::registry