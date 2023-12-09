#pragma once
#include "mgl_opengl/attachment.hpp"
#include "mgl_opengl/attribute.hpp"
#include "mgl_opengl/buffer.hpp"
#include "mgl_opengl/buffer_layout.hpp"
#include "mgl_opengl/builtins.hpp"
#include "mgl_opengl/compute_shader.hpp"
#include "mgl_opengl/conditional_render.hpp"
#include "mgl_opengl/context.hpp"
#include "mgl_opengl/data_type.hpp"
#include "mgl_opengl/framebuffer.hpp"
#include "mgl_opengl/glsl_source.hpp"
#include "mgl_opengl/program.hpp"
#include "mgl_opengl/query.hpp"
#include "mgl_opengl/renderbuffer.hpp"
#include "mgl_opengl/sampler.hpp"
#include "mgl_opengl/scope.hpp"
#include "mgl_opengl/subroutine.hpp"
#include "mgl_opengl/texture.hpp"
#include "mgl_opengl/texture_2d.hpp"
#include "mgl_opengl/texture_3d.hpp"
#include "mgl_opengl/texture_array.hpp"
#include "mgl_opengl/texture_cube.hpp"
#include "mgl_opengl/uniform.hpp"
#include "mgl_opengl/uniform_block.hpp"
#include "mgl_opengl/varying.hpp"
#include "mgl_opengl/vertex_array.hpp"

namespace mgl::opengl
{
  inline context_ref create_context(context_mode::Enum mode, int required = 330)
  {
    return context::create_context(mode, required);
  }
} // namespace mgl::opengl