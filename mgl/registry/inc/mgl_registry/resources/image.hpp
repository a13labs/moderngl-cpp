#pragma once

#include "mgl_core/math.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_registry/location.hpp"
#include "mgl_registry/resource.hpp"

#include "glm/glm.hpp"

namespace mgl::registry
{
  class image_loader;

  class image;
  using image_ref = mgl::ref<image>;

  class image : public resource
  {
public:
    image(uint32_t width, uint32_t height, uint32_t channels, mgl::uint8_buffer& data)
        : m_width(width)
        , m_height(height)
        , m_channels(channels)
        , m_data(std::move(data))
    { }

    image(u_int32_t width, u_int32_t height, u_int32_t channels);
    image(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data);
    virtual ~image() = default;

    image(const image&) = delete;
    image(image&&) = delete;
    image& operator=(const image&) = delete;
    image& operator=(image&&) = delete;

    virtual resource::type get_type() const override { return resource::type::image; }

    uint32_t width() const { return m_width; }
    uint32_t height() const { return m_height; }
    uint32_t channels() const { return m_channels; }
    mgl::uint8_buffer& buffer() { return m_data; }
    unsigned char* data() { return m_data.data(); }

    void blit(uint32_t x, uint32_t y, const image& image);
    void blit(uint32_t x, uint32_t y, const image_ref& image) { blit(x, y, *image); }

    image_ref crop(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const;
    image_ref crop(mgl::rect rect) const { return crop(rect.x, rect.y, rect.width, rect.height); }

    image_ref flip_vertically() const;
    image_ref flip_horizontally() const;

    image_ref clone() const;

    void put_pixel(uint32_t x, uint32_t y, const glm::vec4& color);
    void put_pixel(uint32_t x, uint32_t y, const glm::vec3& color)
    {
      put_pixel(x, y, glm::vec4(color, 1.0f));
    }

    glm::vec4 get_pixel(uint32_t x, uint32_t y) const;

    void fill(const glm::vec4& color);
    void fill(const glm::vec3& color) { fill(glm::vec4(color, 1.0f)); }

    uint32_t size() const { return m_width * m_height * m_channels; }

    void save(const std::string& path, const location_ref& location = nullptr) const;

private:
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    uint32_t m_channels = 0;
    mgl::uint8_buffer m_data;
  };

} // namespace mgl::registry