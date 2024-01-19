#include "mgl_graphics/enums.hpp"

namespace mgl::graphics
{
  const static mgl::opengl::blend_factor s_blend_equation_mapping[] = {
    mgl::opengl::blend_factor::ZERO,
    mgl::opengl::blend_factor::ONE,
    mgl::opengl::blend_factor::SRC_COLOR,
    mgl::opengl::blend_factor::ONE_MINUS_SRC_COLOR,
    mgl::opengl::blend_factor::DST_COLOR,
    mgl::opengl::blend_factor::ONE_MINUS_DST_COLOR,
    mgl::opengl::blend_factor::SRC_ALPHA,
    mgl::opengl::blend_factor::ONE_MINUS_SRC_ALPHA,
    mgl::opengl::blend_factor::DST_ALPHA,
    mgl::opengl::blend_factor::ONE_MINUS_DST_ALPHA,
    mgl::opengl::blend_factor::CONSTANT_COLOR,
    mgl::opengl::blend_factor::ONE_MINUS_CONSTANT_COLOR,
    mgl::opengl::blend_factor::CONSTANT_ALPHA,
    mgl::opengl::blend_factor::ONE_MINUS_CONSTANT_ALPHA,
    mgl::opengl::blend_factor::SRC_ALPHA_SATURATE,
    mgl::opengl::blend_factor::SRC1_COLOR,
    mgl::opengl::blend_factor::ONE_MINUS_SRC1_COLOR,
    mgl::opengl::blend_factor::SRC1_ALPHA,
    mgl::opengl::blend_factor::ONE_MINUS_SRC1_ALPHA,
  };

  const static mgl::opengl::blend_equation_mode s_blend_equation_mode[] = {
    mgl::opengl::blend_equation_mode::ADD,
    mgl::opengl::blend_equation_mode::SUBTRACT,
    mgl::opengl::blend_equation_mode::REVERSE_SUBTRACT,
    mgl::opengl::blend_equation_mode::MIN,
    mgl::opengl::blend_equation_mode::MAX,
  };

  const static mgl::opengl::compare_func s_compare_func[] = {
    mgl::opengl::compare_func::NONE,       mgl::opengl::compare_func::NEVER,
    mgl::opengl::compare_func::LESS,       mgl::opengl::compare_func::EQUAL,
    mgl::opengl::compare_func::LESS_EQUAL, mgl::opengl::compare_func::GREATER,
    mgl::opengl::compare_func::NOT_EQUAL,  mgl::opengl::compare_func::GREATER_EQUAL,
    mgl::opengl::compare_func::ALWAYS,
  };

  const static mgl::opengl::render_mode s_render_mode[] = {
    mgl::opengl::render_mode::POINTS,       mgl::opengl::render_mode::LINES,
    mgl::opengl::render_mode::LINE_LOOP,    mgl::opengl::render_mode::LINE_STRIP,
    mgl::opengl::render_mode::TRIANGLES,    mgl::opengl::render_mode::TRIANGLE_STRIP,
    mgl::opengl::render_mode::TRIANGLE_FAN,
  };

  mgl::opengl::compare_func to_api(compare_func func)
  {
    return s_compare_func[static_cast<int>(func)];
  }

  mgl::opengl::blend_factor to_api(blend_factor factor)
  {
    return s_blend_equation_mapping[static_cast<int>(factor)];
  }

  mgl::opengl::blend_equation_mode to_api(blend_equation_mode mode)
  {
    return s_blend_equation_mode[static_cast<int>(mode)];
  }

  mgl::opengl::render_mode to_api(render_mode mode)
  {
    return s_render_mode[static_cast<int>(mode)];
  }

} // namespace mgl::graphics