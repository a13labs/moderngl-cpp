/**
 * @file debug.hpp
 * @brief Contains debug-related macros and functions.
 */
#pragma once

#include "log.hpp"

#ifdef MGL_DEBUG
#  if defined(MGL_PLATFORM_WINDOWS)
#    define MGL_DEBUGBREAK() __debugbreak()
#  elif defined(MGL_PLATFORM_LINUX) or defined(MGL_PLATFORM_MACOS)
#    include <signal.h>
#    define MGL_DEBUGBREAK() raise(SIGTRAP)
#  else
#    error "Platform doesn't support debugbreak yet!"
#  endif
#  define MGL_CORE_ENABLE_ASSERTS
#else
#  define MGL_DEBUGBREAK()
#endif

#ifdef MGL_CORE_ENABLE_ASSERTS
#  define MGL_CORE_ASSERT(x, ...)                                                                  \
    {                                                                                              \
      if(!(x))                                                                                     \
      {                                                                                            \
        MGL_CORE_TRACE("Assertion Failed: {0}", __VA_ARGS__);                                      \
        MGL_DEBUGBREAK();                                                                          \
      }                                                                                            \
    }
#else
#  define MGL_CORE_ASSERT(x, y, ...)
#endif
