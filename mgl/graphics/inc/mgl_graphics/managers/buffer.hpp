#pragma once

#include "mgl_graphics/buffer.hpp"
#include "mgl_graphics/manager.hpp"

namespace mgl::graphics
{
  class buffer_manager : public manager<buffer_ref>
  {
public:
    buffer_manager() = default;
    ~buffer_manager() = default;

    virtual void on_add(const buffer_ref& buffer, const std::string& name) override final
    {
      buffer->allocate();
    }
    virtual void on_remove(const buffer_ref& buffer, const std::string& name) override final
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
