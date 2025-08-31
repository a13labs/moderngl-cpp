#pragma once

#include "mgl_platform/api/render_api.hpp"

namespace mgl::platform::api::resources {

    inline program_ref create_program(const std::string& vs_source,
                                      const std::string& fs_source,
                                      const std::string& gs_source = "",
                                      const std::string& tes_source = "",
                                      const std::string& tcs_source = "")
    {
      return render_api::instance().api_create_program(
          vs_source, fs_source, gs_source, tes_source, tcs_source);
    }

    inline texture_2d_ref
    create_texture_2d(int32_t width, int32_t height, int32_t components, int32_t samples = 0)
    {
      return render_api::instance().api_create_texture_2d(width, height, components, samples);
    }

    inline texture_2d_ref create_texture_2d(int32_t width,
                                            int32_t height,
                                            int32_t components,
                                            const mgl::uint8_buffer& data,
                                            int32_t samples = 0)
    {
      MGL_CORE_ASSERT(data.size() > 0, "Invalid texture data");
      MGL_CORE_ASSERT(data.size() == width * height * components, "Invalid texture data size");
      auto texture =
          render_api::instance().api_create_texture_2d(width, height, components, samples);
      texture->upload(data, { 0, 0, width, height });
      return texture;
    }

    inline texture_2d_ref create_texture_2d(const mgl::registry::image_ref& image,
                                            int32_t samples = 0)
    {
      auto texture = render_api::instance().api_create_texture_2d(
          image->width(), image->height(), image->channels(), samples);
      texture->upload(image, { 0, 0, image->width(), image->height() });
      return texture;
    }

    inline index_buffer_ref
    create_index_buffer(const uint16_buffer& data, uint16_t element_size = 4, bool dynamic = false)
    {
      MGL_CORE_ASSERT(data.size() > 0, "Invalid index buffer data");
      auto buffer = render_api::instance().api_create_index_buffer(
          data.size() * sizeof(uint16_t), element_size, dynamic);
      MGL_CORE_ASSERT(buffer != nullptr, "Index buffer is null");
      buffer->upload(data);
      return buffer;
    }

    inline index_buffer_ref
    create_index_buffer(const uint32_buffer& data, uint16_t element_size = 4, bool dynamic = false)
    {
      MGL_CORE_ASSERT(data.size() > 0, "Invalid index buffer data");
      auto buffer = render_api::instance().api_create_index_buffer(
          data.size() * sizeof(uint32_t), element_size, dynamic);
      MGL_CORE_ASSERT(buffer != nullptr, "Index buffer is null");
      buffer->upload(data);
      return buffer;
    }

    inline index_buffer_ref
    create_index_buffer(size_t size, uint16_t element_size = 4, bool dynamic = false)
    {
      return render_api::instance().api_create_index_buffer(size, element_size, dynamic);
    }

    inline vertex_buffer_ref create_vertex_buffer(const float32_buffer& data,
                                                  const std::string& layout,
                                                  mgl::string_list attrs,
                                                  bool dynamic = false)
    {
      MGL_CORE_ASSERT(data.size() > 0, "Invalid vertex buffer data");
      auto buffer = render_api::instance().api_create_vertex_buffer(
          layout, attrs, data.size() * sizeof(float), dynamic);
      MGL_CORE_ASSERT(buffer != nullptr, "Vertex buffer is null");
      buffer->upload(data);
      return buffer;
    }

    inline vertex_buffer_ref
    create_vertex_buffer(const std::string& layout, mgl::string_list attrs, bool dynamic = false)
    {
      return render_api::instance().api_create_vertex_buffer(layout, attrs, 0, dynamic);
    }

    inline vertex_buffer_ref create_vertex_buffer(size_t size,
                                                  const std::string& layout,
                                                  mgl::string_list attrs,
                                                  bool dynamic = false)
    {
      return render_api::instance().api_create_vertex_buffer(layout, attrs, size, dynamic);
    }

    inline buffer_ref create_buffer(size_t size, bool dynamic = false)
    {
      return render_api::instance().api_create_buffer(size, dynamic);
    }


} // namespace mgl::platform::api::resources