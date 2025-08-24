#import <Foundation/Foundation.h>
#import <Metal/Metal.h>

#include "mgl_core/debug.hpp"
#include "mgl_metal/context.hpp"
#include "mgl_metal/program.hpp"

namespace mgl::metal {
program::program(const context_ref &ctx, const shaders &shaders,
                 const shaders_outputs &outputs,
                 const fragment_outputs &fragment_outputs, bool interleave,
                 const std::string &filename)
    : object(ctx), m_native_render_pipeline(nullptr),
      m_native_vertex_function(nullptr), m_native_fragment_function(nullptr),
      m_native_library(nullptr) {
  MGL_CORE_ASSERT(ctx && ctx->is_valid(), "Invalid context");

  id<MTLDevice> device = (__bridge id<MTLDevice>)ctx->device();

  // TODO: Compile Metal shaders from source
  // For now, we'll create a placeholder implementation

  // Create a default library (this would normally be compiled from shader
  // source)
  NSError *error = nil;
  id<MTLLibrary> library = [device newDefaultLibrary];

  if (library == nil) {
    MGL_CORE_ASSERT(false, "Failed to create Metal library");
    return;
  }

  m_native_library = (__bridge void *)library;

  // Get vertex and fragment functions
  // TODO: These names should come from the compiled shader
  id<MTLFunction> vertexFunction = [library newFunctionWithName:@"vertex_main"];
  id<MTLFunction> fragmentFunction =
      [library newFunctionWithName:@"fragment_main"];

  if (vertexFunction && fragmentFunction) {
    m_native_vertex_function = (__bridge void *)vertexFunction;
    m_native_fragment_function = (__bridge void *)fragmentFunction;

    // Create render pipeline state
    MTLRenderPipelineDescriptor *pipelineDescriptor =
        [[MTLRenderPipelineDescriptor alloc] init];
    pipelineDescriptor.vertexFunction = vertexFunction;
    pipelineDescriptor.fragmentFunction = fragmentFunction;
    pipelineDescriptor.colorAttachments[0].pixelFormat =
        MTLPixelFormatBGRA8Unorm;

    id<MTLRenderPipelineState> pipelineState =
        [device newRenderPipelineStateWithDescriptor:pipelineDescriptor
                                               error:&error];

    if (pipelineState) {
      m_native_render_pipeline = (__bridge void *)pipelineState;
    } else {
      MGL_CORE_ASSERT(false, "Failed to create render pipeline state");
    }
  }

  // Initialize uniforms and attributes
  init_uniforms();
  init_attributes();
}

void program::release() {
  if (m_native_render_pipeline) {
    CFRelease(m_native_render_pipeline);
    m_native_render_pipeline = nullptr;
  }

  if (m_native_vertex_function) {
    CFRelease(m_native_vertex_function);
    m_native_vertex_function = nullptr;
  }

  if (m_native_fragment_function) {
    CFRelease(m_native_fragment_function);
    m_native_fragment_function = nullptr;
  }

  if (m_native_library) {
    CFRelease(m_native_library);
    m_native_library = nullptr;
  }
}

void program::bind() {
  // TODO: Set the render pipeline state
  // This will be handled by the render encoder in Metal
}

void program::unbind() {
  // TODO: Unbind the render pipeline state
  // This will be handled by the render encoder in Metal
}

void program::init_uniforms() {
  // TODO: Parse Metal shader reflection to discover uniforms
  // For now, this is a placeholder
}

void program::init_attributes() {
  // TODO: Parse Metal shader reflection to discover vertex attributes
  // For now, this is a placeholder
}

} // namespace mgl::metal
