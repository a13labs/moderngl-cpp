/*
   Copyright 2022 Alexandre Pires (c.alexandre.pires@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
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
  class texture_array : public gl_object
  {
public:
    ~texture_array() = default;

    virtual void release() override;

    int32_t width() const { return m_width; }

    int32_t height() const { return m_height; }

    int32_t layers() const { return m_layers; }

    int32_t components() const { return m_components; }

    bool repeat_x() const { return m_repeat_x; }

    void set_repeat_x(bool value);

    bool repeat_y() const { return m_repeat_y; }

    void set_repeat_y(bool value);

    const texture::filter& filter() const { return m_filter; }

    void set_filter(const texture::filter& value);

    std::string swizzle();

    void set_swizzle(const std::string& value);

    float anisotropy() const { return m_anisotropy; }

    void set_anisotropy(float value);

    void read(mgl::uint8_buffer& dst, int32_t align = 1, size_t dst_off = 0);

    void read(buffer_ref& dst, int32_t align = 1, size_t dst_off = 0);

    void write(const mgl::uint8_buffer& src, const mgl::cube& v, int32_t align = 1);

    void write(const mgl::uint8_buffer& src, int32_t align = 1);

    void write(const buffer_ref& src, const mgl::cube& v, int32_t align = 1);

    void write(const buffer_ref& src, int32_t align = 1);

    void bind_to_image(
        int32_t unit, bool read = true, bool write = true, int32_t lvl = 0, int32_t format = 0);

    void build_mipmaps(int32_t base = 0, int32_t max_lvl = 1000);

    void use(int32_t index = 0);

private:
    friend class context;

    texture_array(const context_ref& ctx,
                  int32_t w,
                  int32_t h,
                  int32_t layers,
                  int32_t components,
                  const void* data,
                  int32_t align,
                  const std::string& dtype);

    data_type* m_data_type;
    int32_t m_width;
    int32_t m_height;
    int32_t m_layers;
    int32_t m_components;
    texture::filter m_filter;
    int32_t m_max_level;
    bool m_repeat_x;
    bool m_repeat_y;
    float m_anisotropy;
  };

  using texture_array_ref = mgl::ref<texture_array>;

} // namespace  mgl::opengl