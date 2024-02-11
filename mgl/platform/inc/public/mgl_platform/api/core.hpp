#pragma once

#include "mgl_opengl/context.hpp"

namespace mgl::platform::api
{
  // Map the opengl namespace to the api namespace, eventually we might have a vulkan api
  using context_ref = mgl::opengl::context_ref;
  using texture_ref = mgl::opengl::texture_ref;
  using texture_2d_ref = mgl::opengl::texture_2d_ref;
  using buffer_ref = mgl::opengl::buffer_ref;
  using framebuffer_ref = mgl::opengl::framebuffer_ref;
  using program_ref = mgl::opengl::program_ref;
  using uniform_ref = mgl::opengl::uniform_ref;

  class vertex_array
  {
public:
    vertex_array(const buffer_ref& vb, const std::string& layout, uint32_t mode)
        : m_vb(vb)
        , m_layout(layout)
        , m_ib(nullptr)
        , m_element_size(0)
        , m_mode(mode)
    { }

    vertex_array(const buffer_ref& vb,
                 const std::string& layout,
                 const buffer_ref& ib,
                 uint16_t element_size,
                 uint32_t mode)
        : m_vb(vb)
        , m_layout(layout)
        , m_ib(ib)
        , m_element_size(element_size)
        , m_mode(mode)
    { }

    virtual ~vertex_array() = default;

    vertex_array(const vertex_array& other) = delete;

    virtual void allocate(program_ref program = nullptr) = 0;

    virtual void deallocate() = 0;

    virtual void draw(const glm::mat4& model_view, int count, int offset, int instance_count) = 0;

    virtual bool is_allocated() const = 0;

protected:
    buffer_ref m_vb = nullptr;
    buffer_ref m_ib = nullptr;
    std::string m_layout = "";
    uint16_t m_element_size = 0;
    uint32_t m_mode = 0;
  };

  using vertex_array_ref = std::shared_ptr<vertex_array>;

} // namespace mgl::platform::api