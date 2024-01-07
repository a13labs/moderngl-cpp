#include "mgl_graphics/commands/functions.hpp"
#include "mgl_core/debug.hpp"
#include <glm/gtx/string_cast.hpp>

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

  void clear_command::execute()
  {
    auto& render = mgl::graphics::current_render();
    MGL_CORE_ASSERT(render.context() != nullptr, "Context is null");
    render.context()->clear(m_color);
  }

  void set_view_command::execute()
  {
    auto& render = mgl::graphics::current_render();
    render.current_state().view_matrix = m_view;
    if(render.current_state().view_uniform != nullptr)
    {
      render.current_state().view_uniform->set_value(m_view);
    }
  }

  void set_projection_command::execute()
  {
    auto& render = mgl::graphics::current_render();
    render.current_state().projection_matrix = m_projection;
    if(render.current_state().projection_uniform != nullptr)
    {
      render.current_state().projection_uniform->set_value(m_projection);
    }
  }

  void set_blend_func_command::execute()
  {
    auto& render = mgl::graphics::current_render();
    auto ctx = render.context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->set_blend_func(s_blend_equation_mapping[m_srcRGB],
                        s_blend_equation_mapping[m_dstRGB],
                        s_blend_equation_mapping[m_srcAlpha],
                        s_blend_equation_mapping[m_dstAlpha]);
  }

  void set_blend_equation_command::execute()
  {
    auto& render = mgl::graphics::current_render();
    auto ctx = render.context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->set_blend_equation(s_blend_equation_mode[m_modeRGB], s_blend_equation_mode[m_modeAlpha]);
  }

  void clear_samplers_command::execute()
  {
    auto& render = mgl::graphics::current_render();
    auto ctx = render.context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->clear_samplers(m_start, m_end);
  }
} // namespace mgl::graphics