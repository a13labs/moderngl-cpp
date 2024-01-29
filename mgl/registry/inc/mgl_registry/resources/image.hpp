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
    image(int32_t width, int32_t height, int32_t channels, mgl::uint8_buffer& data)
        : m_width(width)
        , m_height(height)
        , m_channels(channels)
        , m_data(std::move(data))
    { }

    image(int32_t width, int32_t height, int32_t channels);
    image(int32_t width, int32_t height, int32_t channels, unsigned char* data);
    virtual ~image() = default;

    image(const image&) = delete;
    image(image&&) = delete;
    image& operator=(const image&) = delete;
    image& operator=(image&&) = delete;

    virtual resource::type get_type() const override { return resource::type::image; }

    int32_t width() const { return m_width; }
    int32_t height() const { return m_height; }
    int32_t channels() const { return m_channels; }
    mgl::uint8_buffer& buffer() { return m_data; }
    unsigned char* data() { return m_data.data(); }

    void blit(int32_t x, int32_t y, const image& image, const mgl::rect& rect);
    void blit(int32_t x, int32_t y, const image& image)
    {
      blit(x, y, image, { 0, 0, image.width(), image.height() });
    }
    void blit(int32_t x, int32_t y, const image_ref& image, const mgl::rect& rect)
    {
      blit(x, y, *image, rect);
    }
    void blit(int32_t x, int32_t y, const image_ref& image) { blit(x, y, *image); }
    void blit(glm::vec2 position, const image& image) { blit(position.x, position.y, image); }
    void blit(glm::vec2 position, const image_ref& image) { blit(position.x, position.y, image); }

    image_ref crop(int32_t x, int32_t y, int32_t width, int32_t height) const;
    image_ref crop(mgl::rect rect) const { return crop(rect.x, rect.y, rect.width, rect.height); }

    image_ref flip_vertically() const;
    image_ref flip_horizontally() const;

    image_ref clone() const;

    void resize(int32_t width, int32_t height);
    void resize(glm::vec2 size) { resize(size.x, size.y); }

    void put_pixel(int32_t x, int32_t y, const glm::vec4& color);
    void put_pixel(int32_t x, int32_t y, const glm::vec3& color)
    {
      put_pixel(x, y, glm::vec4(color, 1.0f));
    }

    glm::vec4 get_pixel(int32_t x, int32_t y) const;

    void fill(const glm::vec4& color);
    void fill(const glm::vec3& color) { fill(glm::vec4(color, 1.0f)); }

    int32_t size() const { return m_width * m_height * m_channels; }

    void save(const std::string& path, const location_ref& location = nullptr) const;

private:
    int32_t m_width = 0;
    int32_t m_height = 0;
    int32_t m_channels = 0;
    mgl::uint8_buffer m_data;
  };

} // namespace mgl::registry