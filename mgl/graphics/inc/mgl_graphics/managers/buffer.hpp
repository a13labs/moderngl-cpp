#pragma once

#include "mgl_graphics/buffer.hpp"
#include "mgl_graphics/manager.hpp"

namespace mgl::graphics
{
  class buffer_manager : public manager<buffer_ref>
  {
public:
    buffer_manager();
    ~buffer_manager();

    virtual bool on_add(const buffer_ref& buffer) override final;
    virtual void on_remove(const buffer_ref& buffer) override final;

    static buffer_manager& instance();
  };

  inline buffer_manager& current_buffer_manager()
  {
    return buffer_manager::instance();
  }
} // namespace mgl::graphics
