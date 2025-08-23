#import <Metal/Metal.h>
#import <Foundation/Foundation.h>

#include "mgl_metal/texture_cube.hpp"
#include "mgl_metal/context.hpp"
#include "mgl_core/debug.hpp"

namespace mgl::metal
{
  texture_cube::texture_cube(const context_ref& ctx,
                             int32_t size,
                             int32_t components,
                             const void* data,
                             int32_t samples,
                             int32_t align,
                             const std::string& dtype)
      : m_ctx(ctx)
      , m_native_texture(nullptr)
      , m_size(size)
      , m_depth(false)
      , m_samples(samples)
      , m_components(components)
      , m_filter({0, 0})
      , m_max_lvl(0)
      , m_compare_func(compare_func::NEVER)
      , m_anisotropy(1.0f)
  {
    MGL_CORE_ASSERT(ctx && ctx->is_valid(), "Invalid context");
    
    id<MTLDevice> device = (__bridge id<MTLDevice>)ctx->device();
    
    MTLTextureDescriptor* descriptor = [[MTLTextureDescriptor alloc] init];
    descriptor.textureType = MTLTextureTypeCube;
    descriptor.width = size;
    descriptor.height = size;
    descriptor.depth = 1;
    descriptor.mipmapLevelCount = 1;
    descriptor.sampleCount = samples > 0 ? samples : 1;
    descriptor.arrayLength = 6; // 6 faces for cube map
    
    // Set pixel format based on components
    switch(components)
    {
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
    m_native_texture = (__bridge void*)texture;
    
    if(data)
    {
      // Upload initial data for all 6 faces
      NSUInteger bytesPerRow = size * components;
      NSUInteger bytesPerImage = bytesPerRow * size;
      MTLRegion region = MTLRegionMake2D(0, 0, size, size);
      
      const uint8_t* face_data = static_cast<const uint8_t*>(data);
      for(int32_t face = 0; face < 6; ++face)
      {
        [texture replaceRegion:region
                   mipmapLevel:0
                         slice:face
                     withBytes:face_data + (face * bytesPerImage)
                   bytesPerRow:bytesPerRow
                 bytesPerImage:bytesPerImage];
      }
    }
  }

  texture_cube::texture_cube(const context_ref& ctx,
                             int32_t size,
                             const void* data,
                             int32_t samples,
                             int32_t align)
      : texture_cube(ctx, size, 4, data, samples, align, "f1")
  {
  }

  void texture_cube::release()
  {
    if(m_native_texture)
    {
      CFRelease(m_native_texture);
      m_native_texture = nullptr;
    }
  }

  void texture_cube::set_filter(const texture::filter& value)
  {
    m_filter = value;
    // TODO: Update Metal sampler state
  }

  void texture_cube::set_compare_func(mgl::metal::compare_func value)
  {
    m_compare_func = value;
    // TODO: Update Metal sampler state
  }

  void texture_cube::set_anisotropy(float value)
  {
    m_anisotropy = value;
    // TODO: Update Metal sampler state
  }

  std::string texture_cube::swizzle() const
  {
    // TODO: Implement swizzle getter
    return "rgba";
  }

  void texture_cube::set_swizzle(const std::string& value)
  {
    // TODO: Implement swizzle setter
  }

  void texture_cube::read(face f, mgl::uint8_buffer& dst, int32_t lvl, int32_t align, size_t dst_off)
  {
    // TODO: Implement texture read
    MGL_CORE_ASSERT(false, "Not implemented yet");
  }

  void texture_cube::read(face f, buffer_ref& dst, int32_t lvl, int32_t align, size_t dst_off)
  {
    // TODO: Implement texture read to buffer
    MGL_CORE_ASSERT(false, "Not implemented yet");
  }

  void texture_cube::write(face f, const mgl::uint8_buffer& src, const mgl::rect& v, int32_t lvl, int32_t align)
  {
    if(!m_native_texture) return;
    
    id<MTLTexture> texture = (__bridge id<MTLTexture>)m_native_texture;
    NSUInteger bytesPerRow = v.width * m_components;
    NSUInteger bytesPerImage = bytesPerRow * v.height;
    MTLRegion region = MTLRegionMake2D(v.x, v.y, v.width, v.height);
    
    [texture replaceRegion:region
               mipmapLevel:lvl
                     slice:static_cast<int32_t>(f)
                 withBytes:src.data()
               bytesPerRow:bytesPerRow
             bytesPerImage:bytesPerImage];
  }

  void texture_cube::write(face f, const mgl::uint8_buffer& src, int32_t lvl, int32_t align)
  {
    mgl::rect full_rect = {0, 0, m_size, m_size};
    write(f, src, full_rect, lvl, align);
  }

  void texture_cube::write(face f, const buffer_ref& src, const mgl::rect& v, int32_t lvl, int32_t align)
  {
    // TODO: Implement buffer to texture write
    MGL_CORE_ASSERT(false, "Not implemented yet");
  }

  void texture_cube::write(face f, const buffer_ref& src, int32_t lvl, int32_t align)
  {
    // TODO: Implement buffer to texture write
    MGL_CORE_ASSERT(false, "Not implemented yet");
  }

  void texture_cube::bind_to_image(int32_t unit, bool read, bool write, int32_t lvl, int32_t f)
  {
    // TODO: Implement image binding
    MGL_CORE_ASSERT(false, "Not implemented yet");
  }

  void texture_cube::build_mipmaps(int32_t base, int32_t max_lvl)
  {
    // TODO: Implement mipmap generation
    MGL_CORE_ASSERT(false, "Not implemented yet");
  }

  void texture_cube::use(int32_t index)
  {
    // TODO: Implement texture binding for use
    // This will be handled by the render encoder in Metal
  }

} // namespace mgl::metal
