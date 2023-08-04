#pragma once

#include "setup.hpp"

#if GLM_SILENT_WARNINGS == GLM_ENABLE
#  if GLM_COMPILER & GLM_COMPILER_GCC
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wvolatile"
#  elif GLM_COMPILER & GLM_COMPILER_CLANG
#    pragma clang diagnostic push
#    pragma GCC diagnostic ignored "-Wdeprecated-volatile"
#  elif GLM_COMPILER & GLM_COMPILER_VC
#    pragma warning(push)
#  endif
#endif

namespace glm
{
  namespace detail
  {
    typedef short hdata;

    GLM_FUNC_DECL float toFloat32(hdata value);
    GLM_FUNC_DECL hdata toFloat16(float const& value);

  } //namespace detail
} //namespace glm

#include "type_half.inl"
