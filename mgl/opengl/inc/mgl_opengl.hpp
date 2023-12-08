#pragma once
#include "mgl_opengl/attachment.hpp"
#include "mgl_opengl/attribute.hpp"
#include "mgl_opengl/buffer.hpp"
#include "mgl_opengl/buffer_layout.hpp"
#include "mgl_opengl/builtins.hpp"
#include "mgl_opengl/computeshader.hpp"
#include "mgl_opengl/conditionalrender.hpp"
#include "mgl_opengl/context.hpp"
#include "mgl_opengl/datatype.hpp"
#include "mgl_opengl/framebuffer.hpp"
#include "mgl_opengl/glslsource.hpp"
#include "mgl_opengl/program.hpp"
#include "mgl_opengl/query.hpp"
#include "mgl_opengl/renderbuffer.hpp"
#include "mgl_opengl/sampler.hpp"
#include "mgl_opengl/scope.hpp"
#include "mgl_opengl/subroutine.hpp"
#include "mgl_opengl/texture.hpp"
#include "mgl_opengl/texture2d.hpp"
#include "mgl_opengl/texture3d.hpp"
#include "mgl_opengl/texturearray.hpp"
#include "mgl_opengl/texturecube.hpp"
#include "mgl_opengl/uniform.hpp"
#include "mgl_opengl/uniformblock.hpp"
#include "mgl_opengl/varying.hpp"
#include "mgl_opengl/vertexarray.hpp"

namespace mgl::opengl
{
  inline context_ref create_context(context_mode::Enum mode, int required = 330)
  {
    return context::create_context(mode, required);
  }
} // namespace mgl::opengl