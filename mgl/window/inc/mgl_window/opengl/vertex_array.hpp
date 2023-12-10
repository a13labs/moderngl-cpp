#pragma once
#include "enums.hpp"
#include "vertex_buffer.hpp"

#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"

#include "mgl_opengl/program.hpp"
#include "mgl_opengl/vertex_array.hpp"

namespace mgl::window::opengl
{
  class vertex_array
  {
public:
    vertex_array(const std::string& name, render_mode mode = render_mode::TRIANGLES);
    ~vertex_array() = default;

    void render(const mgl::opengl::program_ref& program,
                render_mode mode = render_mode::NONE,
                int vertices = -1,
                int first = 0,
                int instances = -1);

    void render_indirect(const mgl::opengl::program_ref& program,
                         const mgl::opengl::buffer_ref& buffer,
                         render_mode mode,
                         int count = -1,
                         int first = -1);

    void transform(const mgl::opengl::program_ref& program,
                   const mgl::opengl::buffer_ref& buffer,
                   render_mode mode,
                   int vertices = -1,
                   int first = 0,
                   int instances = -1);

    void index_buffer(const mgl::buffer<float>& data, int index_element_size = 4);
    void index_buffer(const mgl::buffer<uint32_t>& data, int index_element_size = 4);
    void index_buffer(const mgl::buffer<uint16_t>& data, int index_element_size = 4);
    void index_buffer(const mgl::buffer<uint8_t>& data, int index_element_size = 4);
    void index_buffer(const mgl::opengl::buffer_ref& buffer, int index_element_size = 4);

    void buffer(const mgl::buffer<float>& data,
                const std::string& layout,
                const mgl::string_list& attrs);

    void buffer(const mgl::buffer<uint32_t>& data,
                const std::string& layout,
                const mgl::string_list& attrs);

    void buffer(const mgl::buffer<uint16_t>& data,
                const std::string& layout,
                const mgl::string_list& attrs);

    void buffer(const mgl::buffer<uint8_t>& data,
                const std::string& layout,
                const mgl::string_list& attrs);

    void buffer(const mgl::opengl::buffer_ref& buffer,
                const std::string& layout,
                const mgl::string_list& attrs);

    mgl::opengl::vertex_array_ref instance(const mgl::opengl::program_ref& program);

private:
    std::string m_name;
    render_mode m_mode;
    mgl::dict<int, mgl::opengl::vertex_array_ref> m_vao_cache;
    vertex_buffers_list m_buffers;
    int m_vertex_count;
    mgl::opengl::buffer_ref m_index_buffer;
    int m_index_element_size;
  };

} // namespace mgl::window::opengl
