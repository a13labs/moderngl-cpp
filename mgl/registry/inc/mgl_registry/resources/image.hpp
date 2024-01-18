#pragma once

#include "mgl_core/memory.hpp"
#include "mgl_registry/resource.hpp"

namespace mgl::registry
{
  class image_loader;
  class image;
  using image_ref = mgl::ref<image>;

  class image : public resource
  {
public:
    virtual ~image() = default;

    image(int width, int height, int channels, mgl::byte_buffer& data)
        : m_width(width)
        , m_height(height)
        , m_channels(channels)
        , m_data(std::move(data))
    { }
    image(const image&) = delete;
    image(image&&) = delete;
    image& operator=(const image&) = delete;
    image& operator=(image&&) = delete;

    virtual resource::type get_type() const override { return resource::type::image; }

    int width() const { return m_width; }
    int height() const { return m_height; }
    int channels() const { return m_channels; }
    const mgl::byte_buffer& data() const { return m_data; }

private:
    int m_width = 0;
    int m_height = 0;
    int m_channels = 0;
    mgl::byte_buffer m_data;
  };

} // namespace mgl::registry