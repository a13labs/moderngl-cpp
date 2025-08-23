#include "mgl_metal/context.hpp"
#include "mgl_metal/buffer.hpp"
#include "mgl_metal/texture_2d.hpp"
#include "mgl_metal/texture_3d.hpp"
#include "mgl_metal/texture_cube.hpp"
#include "mgl_metal/program.hpp"

#include "mgl_core/log.hpp"
#include "mgl_core/debug.hpp"

#ifdef __OBJC__
#import <Metal/Metal.h>
#endif

namespace mgl::metal {

context::context(context_mode::mode mode, int required) {
    id<MTLDevice> device = nil;

    switch (mode) {
        case context_mode::STANDALONE:
        case context_mode::DEFAULT:
            device = MTLCreateSystemDefaultDevice();
            break;
        case context_mode::SHARE:
        case context_mode::ATTACHED:
            MGL_CORE_ASSERT(false, "[Metal Context] (share/attached) Mode not supported.")
            break;
    }

    if (!device) {
        MGL_CORE_ASSERT(false, "[Metal Context] Device not available");
    }

    m_device = (__bridge void*)device;
    m_commandQueue = (__bridge void*)[device newCommandQueue];
    m_mode = mode;
    m_released = false;
}

context::~context() {
    release();
}

void context::enter() {
    // Enter the Metal context
    if (m_device) {
        [(id<MTLDevice>)m_device retain];
    }
}

void context::exit() {
    // Exit the Metal context
    if (m_device) {
        [(id<MTLDevice>)m_device release];
    }
}

void context::release() {
    if (!m_released) {
        m_released = true;
        // Release Metal objects
        if (m_commandQueue) {
            m_commandQueue = nil;
        }
        if (m_device) {
            m_device = nil;
        }
    }
}

bool context::is_current() const {
    return m_device == MTLCreateSystemDefaultDevice();
}

buffer_ref context::buffer(void* data, size_t reserve, bool dynamic) {
    MGL_CORE_ASSERT(!released(), "[Metal Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[Metal Context] Resource context not current.");
    auto buffer = new mgl::metal::buffer(shared_from_this(), data, reserve, dynamic);
    return buffer_ref(buffer);
}

texture_2d_ref context::texture_2d(int32_t width,
                                   int32_t height,
                                   int32_t components,
                                   const void* data,
                                   int32_t samples,
                                   int32_t align,
                                   const std::string& dtype) {
    MGL_CORE_ASSERT(!released(), "[Metal Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[Metal Context] Resource context not current.");
    auto texture = new mgl::metal::texture_2d(shared_from_this(), width, height, components, data, samples, align, dtype);
    return texture_2d_ref(texture);
}

texture_3d_ref context::texture_3d(int32_t width,
                                   int32_t height,
                                   int32_t depth,
                                   int32_t components,
                                   const void* data,
                                   int32_t align,
                                   const std::string& dtype) {
    MGL_CORE_ASSERT(!released(), "[Metal Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[Metal Context] Resource context not current.");
    auto texture = new mgl::metal::texture_3d(shared_from_this(), width, height, depth, components, data, align, dtype);
    return texture_3d_ref(texture);
}

texture_cube_ref context::texture_cube(int32_t size,
                                       int32_t components,
                                       const void* data,
                                       int32_t samples,
                                       int32_t align,
                                       const std::string& dtype) {
    MGL_CORE_ASSERT(!released(), "[Metal Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[Metal Context] Resource context not current.");
    auto texture = new mgl::metal::texture_cube(shared_from_this(), size, components, data, samples, align, dtype);
    return texture_cube_ref(texture);
}

program_ref context::program(const std::string& vertex_shader,
                            const std::string& fragment_shader,
                            const std::string& geometry_shader,
                            const std::string& tess_evaluation_shader,
                            const std::string& tess_control_shader,
                            const mgl::dict<std::string, int>& fragment_outputs,
                            const mgl::string_list& varyings_names,
                            bool interleaved) {
    MGL_CORE_ASSERT(!released(), "[Metal Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[Metal Context] Resource context not current.");
    auto prog = new mgl::metal::program(shared_from_this(), vertex_shader, fragment_shader, geometry_shader, 
                                       tess_evaluation_shader, tess_control_shader, fragment_outputs, 
                                       varyings_names, interleaved);
    return program_ref(prog);
}


} // namespace mgl::metal
