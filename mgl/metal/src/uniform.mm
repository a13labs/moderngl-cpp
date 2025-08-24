#import <Foundation/Foundation.h>
#import <Metal/Metal.h>

#include "mgl_core/debug.hpp"
#include "mgl_core/log.hpp"
#include "mgl_metal/context.hpp"
#include "mgl_metal/program.hpp"
#include "mgl_metal/uniform.hpp"

namespace mgl::metal {
uniform::uniform(const std::string &name, int32_t dtype, int32_t program_obj,
                 int32_t location, size_t size) {
  MGL_CORE_ASSERT(false, "Not implemented");
}

void uniform::set_value(void *data, size_t size) {
  MGL_CORE_ASSERT(m_ctx && m_ctx->is_valid(), "Invalid context");
  MGL_CORE_ASSERT(m_program, "Invalid program");
  MGL_CORE_ASSERT(m_native_uniform, "Invalid native uniform");
  MGL_CORE_ASSERT(data, "Invalid data");
  MGL_CORE_ASSERT(size > 0, "Invalid size");

  // Set the uniform value using Metal API
}

void uniform::get_value(void *data, size_t size) {
  MGL_CORE_ASSERT(m_ctx && m_ctx->is_valid(), "Invalid context");
  MGL_CORE_ASSERT(m_program, "Invalid program");
  MGL_CORE_ASSERT(m_native_uniform, "Invalid native uniform");
  MGL_CORE_ASSERT(data, "Invalid data");
  MGL_CORE_ASSERT(size > 0, "Invalid size");

  // Get the uniform value using Metal API
}

} // namespace mgl::metal
