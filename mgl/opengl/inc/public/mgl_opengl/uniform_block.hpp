#pragma once

#include "mgl_core/containers.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"

namespace mgl::opengl
{
  class uniform_block
  {
public:
    uniform_block(const std::string& name, int program_obj, int index, size_t size);
    ~uniform_block() = default;

    int binding();
    void set_binding(int value);

private:
    std::string m_name;
    int m_program_obj;
    int m_index;
    int m_size;
  };

  using uniform_block_ref = mgl::ref<uniform_block>;
  using uniform_blocks_dict = mgl::dict<std::string, uniform_block_ref>;

} // namespace  mgl::opengl
