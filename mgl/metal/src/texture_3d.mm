#import <Foundation/Foundation.h>
#import <Metal/Metal.h>

#include "mgl_core/debug.hpp"
#include "mgl_metal/context.hpp"
#include "mgl_metal/texture_3d.hpp"

namespace mgl::metal {
texture_3d::texture_3d(const context_ref &ctx, int32_t w, int32_t h, int32_t d,
                       int32_t components, const void *data, int32_t align,
                       const std::string &dtype)
    : object(ctx), m_native_texture(nullptr), m_width(w), m_height(h),
      m_depth_size(d), m_samples(1), m_components(components), m_filter({0, 0}),
      m_max_lvl(0), m_anisotropy(1.0f), m_repeat_x(true), m_repeat_y(true),
      m_repeat_z(true) {
  MGL_CORE_ASSERT(ctx && ctx->is_valid(), "Invalid context");

  id<MTLDevice> device = (__bridge id<MTLDevice>)ctx->device();

  MTLTextureDescriptor *descriptor = [[MTLTextureDescriptor alloc] init];
  descriptor.textureType = MTLTextureType3D;
  descriptor.width = w;
  descriptor.height = h;
  descriptor.depth = d;
  descriptor.mipmapLevelCount = 1;
  descriptor.sampleCount = 1;
  descriptor.arrayLength = 1;

  // Set pixel format based on components
  switch (components) {
  case 1:
    descriptor.pixelFormat = MTLPixelFormatR8Unorm;
    break;
  case 2:
    descriptor.pixelFormat = MTLPixelFormatRG8Unorm;
    break;
  case 3:
  case 4:
  default:
    descriptor.pixelFormat = MTLPixelFormatRGBA8Unorm;
    break;
  }

  descriptor.usage = MTLTextureUsageShaderRead | MTLTextureUsageShaderWrite;

  id<MTLTexture> texture = [device newTextureWithDescriptor:descriptor];
  m_native_texture = (__bridge void *)texture;

  if (data) {
    // Upload initial data
    NSUInteger bytesPerRow = w * components;
    NSUInteger bytesPerImage = bytesPerRow * h;
    MTLRegion region = MTLRegionMake3D(0, 0, 0, w, h, d);
    [texture replaceRegion:region
               mipmapLevel:0
                     slice:0
                 withBytes:data
               bytesPerRow:bytesPerRow
             bytesPerImage:bytesPerImage];
  }
}

void texture_3d::release() {
  if (m_native_texture) {
    CFRelease(m_native_texture);
    m_native_texture = nullptr;
  }
}

void texture_3d::set_repeat_x(bool value) {
  m_repeat_x = value;
  // TODO: Update Metal sampler state
}

void texture_3d::set_repeat_y(bool value) {
  m_repeat_y = value;
  // TODO: Update Metal sampler state
}

void texture_3d::set_repeat_z(bool value) {
  m_repeat_z = value;
  // TODO: Update Metal sampler state
}

void texture_3d::set_filter(const texture::filter &value) {
  m_filter = value;
  // TODO: Update Metal sampler state
}

void texture_3d::set_anisotropy(float value) {
  m_anisotropy = value;
  // TODO: Update Metal sampler state
}

std::string texture_3d::swizzle() const {
  // TODO: Implement swizzle getter
  return "rgba";
}

void texture_3d::set_swizzle(const std::string &value) {
  // TODO: Implement swizzle setter
}

void texture_3d::read(mgl::uint8_buffer &dst, int32_t lvl, int32_t align,
                      size_t dst_off) {
  // TODO: Implement texture read
  MGL_CORE_ASSERT(false, "Not implemented yet");
}

void texture_3d::read(buffer_ref &dst, int32_t lvl, int32_t align,
                      size_t dst_off) {
  // TODO: Implement texture read to buffer
  MGL_CORE_ASSERT(false, "Not implemented yet");
}

void texture_3d::write(const mgl::uint8_buffer &src, const mgl::rect &v,
                       int32_t lvl, int32_t align) {
  if (!m_native_texture)
    return;

  id<MTLTexture> texture = (__bridge id<MTLTexture>)m_native_texture;
  NSUInteger bytesPerRow = v.width * m_components;
  NSUInteger bytesPerImage = bytesPerRow * v.height;
  MTLRegion region =
      MTLRegionMake3D(v.x, v.y, 0, v.width, v.height, m_depth_size);

  [texture replaceRegion:region
             mipmapLevel:lvl
                   slice:0
               withBytes:src.data()
             bytesPerRow:bytesPerRow
           bytesPerImage:bytesPerImage];
}

void texture_3d::write(const mgl::uint8_buffer &src, int32_t lvl,
                       int32_t align) {
  mgl::rect full_rect = {0, 0, m_width, m_height};
  write(src, full_rect, lvl, align);
}

void texture_3d::write(const buffer_ref &src, const mgl::rect &v, int32_t lvl,
                       int32_t align) {
  // TODO: Implement buffer to texture write
  MGL_CORE_ASSERT(false, "Not implemented yet");
}

void texture_3d::write(const buffer_ref &src, int32_t lvl, int32_t align) {
  // TODO: Implement buffer to texture write
  MGL_CORE_ASSERT(false, "Not implemented yet");
}

void texture_3d::resize(int32_t w, int32_t h, int32_t d, int32_t components,
                        const mgl::uint8_buffer &data) {
  // TODO: Implement texture resize
  MGL_CORE_ASSERT(false, "Not implemented yet");
}

void texture_3d::bind_to_image(int32_t unit, bool read, bool write, int32_t lvl,
                               int32_t f) {
  // TODO: Implement image binding
  MGL_CORE_ASSERT(false, "Not implemented yet");
}

void texture_3d::build_mipmaps(int32_t base, int32_t max_lvl) {
  // TODO: Implement mipmap generation
  MGL_CORE_ASSERT(false, "Not implemented yet");
}

void texture_3d::use(int32_t index) {
  // TODO: Implement texture binding for use
  // This will be handled by the render encoder in Metal
}

} // namespace mgl::metal
