#pragma once
#include "attachment.hpp"
#include "attribute.hpp"
#include "buffer.hpp"
#include "builtins.hpp"
#include "computeshader.hpp"
#include "conditionalrender.hpp"
#include "context.hpp"
#include "datatype.hpp"
#include "format.hpp"
#include "framebuffer.hpp"
#include "glmethods.hpp"
#include "glslsource.hpp"
#include "opengl.hpp"
#include "program.hpp"
#include "query.hpp"
#include "renderbuffer.hpp"
#include "sampler.hpp"
#include "scope.hpp"
#include "subroutine.hpp"
#include "texture.hpp"
#include "texture2d.hpp"
#include "texture3d.hpp"
#include "texturearray.hpp"
#include "texturecube.hpp"
#include "uniform.hpp"
#include "uniformblock.hpp"
#include "varying.hpp"
#include "vertexarray.hpp"

namespace mgl::opengl
{

  inline context_ref create_context(context_mode::Enum mode, int required = 330)
  {
    return context::create_context(mode, required);
  }
} // namespace mgl::opengl