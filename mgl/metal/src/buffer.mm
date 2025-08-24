#include "mgl_metal/buffer.hpp"
#include "mgl_metal/context.hpp"

#include "mgl_core/debug.hpp"

#include <algorithm>

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#endif

namespace mgl::metal {
buffer::buffer(const context_ref &ctx, const void *data, size_t reserve,
               bool dynamic)
    : object(ctx), m_size(reserve), m_dynamic(dynamic), m_pos(0) {
  id<MTLDevice> device = (__bridge id<MTLDevice>)ctx->device();

  MTLResourceOptions options =
      dynamic ? MTLResourceStorageModeShared : MTLResourceStorageModePrivate;

  if (data) {
    m_handler = (__bridge void *)[device newBufferWithBytes:data
                                                     length:reserve
                                                    options:options];
  } else {
    m_handler =
        (__bridge void *)[device newBufferWithLength:reserve options:options];
  }

  MGL_CORE_ASSERT(m_handler, "[Buffer] Error creating Metal buffer.");
}

void buffer::release() {
  MGL_CORE_ASSERT(!released(),
                  "[Buffer] Resource already released or not valid.");

  if (m_handler) {
    CFRelease(m_handler);
    m_handler = nullptr;
  }
  m_size = 0;
  m_pos = 0;
}

void buffer::download(void *dst, size_t dst_sz, size_t n_bytes, size_t off,
                      size_t dst_off) {
  if (n_bytes == SIZE_MAX) {
    n_bytes = m_size;
  }

  MGL_CORE_ASSERT(!released(),
                  "[Buffer] Resource already released or not valid.");
  MGL_CORE_ASSERT(n_bytes >= 0, "[Buffer] Invalid size: {0}.", n_bytes)
  MGL_CORE_ASSERT(m_size >= off + n_bytes, "[Buffer] Source out of bounds.")
  MGL_CORE_ASSERT(dst_sz >= dst_off + n_bytes,
                  "[Buffer] Destination out of bounds.")

  id<MTLBuffer> metalBuffer = (__bridge id<MTLBuffer>)m_handler;
  void *contents = [metalBuffer contents];

  if (contents) {
    std::copy((char *)contents + off, (char *)contents + off + n_bytes,
              (char *)dst + dst_off);
  } else {
    // For private storage mode, we need to use a blit encoder to copy to a
    // shared buffer
    MGL_CORE_ASSERT(false, "[Buffer] Cannot directly access private buffer "
                           "contents. Use a staging buffer.");
  }
}

void buffer::upload(const void *src, size_t src_sz, size_t off) {
  MGL_CORE_ASSERT(!released(),
                  "[Buffer] Resource already released or not valid.");
  MGL_CORE_ASSERT(src_sz + off <= m_size, "[Buffer] Source out of bounds.")

  id<MTLBuffer> metalBuffer = (__bridge id<MTLBuffer>)m_handler;
  void *contents = [metalBuffer contents];

  if (contents) {
    std::copy((char *)src, (char *)src + src_sz, (char *)contents + off);

    // For shared storage mode, we need to synchronize
    if (m_dynamic) {
      [metalBuffer didModifyRange:NSMakeRange(off, src_sz)];
    }
  } else {
    // For private storage mode, we need to use a blit encoder
    MGL_CORE_ASSERT(false, "[Buffer] Cannot directly access private buffer "
                           "contents. Use a staging buffer.");
  }

  m_pos = off + src_sz;
}

void buffer::clear() {
  MGL_CORE_ASSERT(!released(),
                  "[Buffer] Resource already released or not valid.");

  id<MTLBuffer> metalBuffer = (__bridge id<MTLBuffer>)m_handler;
  void *contents = [metalBuffer contents];

  if (contents) {
    std::fill((char *)contents, (char *)contents + m_size, 0);

    if (m_dynamic) {
      [metalBuffer didModifyRange:NSMakeRange(0, m_size)];
    }
  } else {
    // For private storage mode, we need to use a blit encoder or compute shader
    MGL_CORE_ASSERT(false, "[Buffer] Cannot directly access private buffer "
                           "contents. Use a compute shader or staging buffer.");
  }

  m_pos = 0;
}

void buffer::orphan(size_t size) {
  MGL_CORE_ASSERT(!released(),
                  "[Buffer] Resource already released or not valid.");
  MGL_CORE_ASSERT(ctx() && ctx()->is_current(),
                  "[Buffer] Resource context not current.");

  if (size == SIZE_MAX) {
    size = m_size;
  }

  id<MTLDevice> device = (__bridge id<MTLDevice>)ctx()->device();
  MTLResourceOptions options =
      m_dynamic ? MTLResourceStorageModeShared : MTLResourceStorageModePrivate;

  // Release old buffer and create new one
  if (m_handler) {
    CFRelease(m_handler);
  }
  m_handler =
      (__bridge void *)[device newBufferWithLength:size options:options];
  MGL_CORE_ASSERT(m_handler, "[Buffer] Error creating new Metal buffer.");

  m_size = size;
  m_pos = 0;
}

void buffer::copy_to(const buffer_ref &dst, size_t size, size_t off,
                     size_t dst_off) {
  if (size == SIZE_MAX) {
    size = m_size - off;
  }

  MGL_CORE_ASSERT(dst, "[Buffer] Invalid buffer");
  MGL_CORE_ASSERT(!released() && !dst->released(),
                  "[Buffer] Resource already released or not valid.");
  MGL_CORE_ASSERT(ctx() == dst->ctx(),
                  "[Buffer] Buffers must be in the same context");
  MGL_CORE_ASSERT(ctx() && ctx()->is_current(),
                  "[Buffer] Resource context not current.");
  MGL_CORE_ASSERT((off + size <= m_size && dst_off + size <= dst->m_size),
                  "[Buffer] Buffer overflow.");

  id<MTLDevice> device = (__bridge id<MTLDevice>)ctx()->device();
  id<MTLCommandQueue> commandQueue =
      (__bridge id<MTLCommandQueue>)ctx()->command_queue();
  id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
  id<MTLBlitCommandEncoder> blitEncoder = [commandBuffer blitCommandEncoder];

  id<MTLBuffer> srcBuffer = (__bridge id<MTLBuffer>)m_handler;
  id<MTLBuffer> dstBuffer = (__bridge id<MTLBuffer>)dst->m_handler;

  [blitEncoder copyFromBuffer:srcBuffer
                 sourceOffset:off
                     toBuffer:dstBuffer
            destinationOffset:dst_off
                         size:size];

  [blitEncoder endEncoding];
  [commandBuffer commit];
  [commandBuffer waitUntilCompleted];
}

void buffer::copy(const buffer_ref &src, const buffer_ref &dst, size_t size,
                  size_t off, size_t dst_off) {
  if (size == SIZE_MAX) {
    size = src->m_size - off;
  }

  MGL_CORE_ASSERT(src && dst, "[Buffer] Invalid buffer");
  MGL_CORE_ASSERT(!src->released() && !dst->released(),
                  "[Buffer] Resource already released or not valid.");
  MGL_CORE_ASSERT(src->ctx() == dst->ctx(),
                  "[Buffer] Buffers must be in the same context");
  MGL_CORE_ASSERT(src->ctx() && src->ctx()->is_current(),
                  "[Buffer] Resource context not current.");
  MGL_CORE_ASSERT((off + size <= src->m_size && dst_off + size <= dst->m_size),
                  "[Buffer] Buffer overflow.");

  id<MTLDevice> device = (__bridge id<MTLDevice>)src->ctx()->device();
  id<MTLCommandQueue> commandQueue =
      (__bridge id<MTLCommandQueue>)src->ctx()->command_queue();
  id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
  id<MTLBlitCommandEncoder> blitEncoder = [commandBuffer blitCommandEncoder];

  id<MTLBuffer> srcBuffer = (__bridge id<MTLBuffer>)src->m_handler;
  id<MTLBuffer> dstBuffer = (__bridge id<MTLBuffer>)dst->m_handler;

  [blitEncoder copyFromBuffer:srcBuffer
                 sourceOffset:off
                     toBuffer:dstBuffer
            destinationOffset:dst_off
                         size:size];

  [blitEncoder endEncoding];
  [commandBuffer commit];
  [commandBuffer waitUntilCompleted];
}

} // namespace mgl::metal
