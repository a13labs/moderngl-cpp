#pragma once

#include "mgl_graphics/manager.hpp"

#include "mgl_platform/api/buffers.hpp"

namespace mgl::graphics
{
  class buffer_manager : public manager<mgl::platform::api::buffer_ref>
  {
public:
    buffer_manager() = default;
    ~buffer_manager() = default;

    virtual void on_add(const mgl::platform::api::buffer_ref& buffer,
                        const std::string& name) override final
    {
      buffer->allocate();
    }
    virtual void on_remove(const mgl::platform::api::buffer_ref& buffer,
                           const std::string& name) override final
    {
      buffer->free();
    }

    static buffer_manager& instance()
    {
      static buffer_manager instance;
      return instance;
    }
  };

  inline buffer_manager& buffers()
  {
    return buffer_manager::instance();
  }
} // namespace mgl::graphics
