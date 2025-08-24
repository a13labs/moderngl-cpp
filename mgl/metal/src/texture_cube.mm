#include "mgl_metal/texture_cube.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_metal/context.hpp"

#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
namespace mgl::metal {
texture_cube::texture_cube(const context_ref &ctx, int32_t w, int32_t h,
                           int32_t components, const void *data, int32_t align,
                           const std::string &dtype,
                           int32_t internal_format_override)
    : object(ctx) {
  MGL_CORE_ASSERT(w > 0, "[TextureCube] Width must be greater than 0.");
  MGL_CORE_ASSERT(h > 0, "[TextureCube] Height must be greater than 0.");
  MGL_CORE_ASSERT(components > 0 && components < 5,
                  "[TextureCube] Components must be 1, 2, 3 or 4.");
  MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                  "[TextureCube] Alignment must be 1, 2, 4 or 8.");

  auto dtype_info = from_dtype(dtype);
  if (!dtype_info) {
    MGL_CORE_ASSERT(false, "[TextureCube] Invalid data type got '{0}'.", dtype);
    return;
  }

  m_width = w;
  m_height = h;
  m_components = components;
  m_data_type = dtype_info;
  m_max_lvl = 0; // no mipmaps here by default

  // default sampler choice (OpenGL used GL_LINEAR for float, GL_NEAREST
  // otherwise)
  auto filter = m_data_type->float_type ? MTLSamplerMinMagFilterLinear
                                        : MTLSamplerMinMagFilterNearest;
  m_filter = {static_cast<int>(filter), static_cast<int>(filter)};

  int32_t base_format = m_data_type->base_format[components];
  int32_t internal_format = internal_format_override
                                ? internal_format_override
                                : m_data_type->internal_format[components];

  // Acquire Metal device & optionally command queue from context.
  // TODO: adapt these casts to how your context stores Metal device/queue.
  id<MTLDevice> device =
      (__bridge id<MTLDevice>)
          ctx->device(); // implement metal_device() in context
  id<MTLCommandQueue> queue =
      (__bridge id<MTLCommandQueue>)
          ctx->command_queue(); // implement metal_queue() in context

  MGL_CORE_ASSERT(device != nil,
                  "[TextureCube] Metal device missing in context.");

  // Create texture descriptor for a cube texture
  MTLTextureDescriptor *desc = [MTLTextureDescriptor
      texture2DDescriptorWithPixelFormat:(MTLPixelFormat)m_data_type->d_type
                                   width:w
                                  height:h
                               mipmapped:NO];
  desc.textureType = MTLTextureTypeCube;
  desc.usage = MTLTextureUsageShaderRead | MTLTextureUsageRenderTarget |
               MTLTextureUsageShaderWrite;

  id<MTLTexture> tex = [device newTextureWithDescriptor:desc];
  MGL_CORE_ASSERT(tex != nil,
                  "[TextureCube] Failed to allocate Metal cube texture.");
  m_handler =
      (__bridge void *)tex; // store opaque pointer (adjust type in header)

  // Upload face data if provided. Data layout expected to contain 6 faces in
  // order.
  if (data) {
    // calculate bytesPerRow with alignment
    size_t bytesPerPixel = m_components * m_data_type->size;
    // If m_data_type->size already includes component count handle properly;
    // above is conservative.
    size_t rowBytes = w * bytesPerPixel;
    // align rowBytes to 'align'
    rowBytes = ((rowBytes + align - 1) / align) * align;

    const char *base = (const char *)data;
    size_t face_stride = rowBytes * h;

    for (int face = 0; face < 6; ++face) {
      MTLRegion region = MTLRegionMake2D(0, 0, w, h);
      const void *face_ptr = base + face_stride * face;
      // For cube textures, slice is face index
      [tex replaceRegion:region
             mipmapLevel:0
                   slice:face
               withBytes:face_ptr
             bytesPerRow:rowBytes
           bytesPerImage:face_stride];
    }
  }
}

void texture_cube::release() {
  MGL_CORE_ASSERT(!released(),
                  "[TextureCube] Resource already released or not valid.");
  // release retained texture
  if (m_handler) {
    id<MTLTexture> tex = (id<MTLTexture>)m_handler;
    (void)tex; // tex will be released when bridge_transfer goes out of scope
    m_handler = nullptr;
  }
}

void texture_cube::set_filter(const texture::filter &value) {
  m_filter = value;
  // TODO: Update Metal sampler state
}

void texture_cube::set_anisotropy(float value) {
  m_anisotropy = value;
  // TODO: Update Metal sampler state
}

void texture_cube::read(mgl::uint8_buffer &dst, int face, int align,
                        size_t write_offset) {
  MGL_CORE_ASSERT(!released(),
                  "[TextureCube] Resource already released or not valid.");
  MGL_CORE_ASSERT(face >= 0 && face <= 5, "[TextureCube] Face must be 0..5.");
  MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                  "[TextureCube] Alignment must be 1, 2, 4 or 8.");

  id<MTLTexture> tex = (__bridge id<MTLTexture>)m_handler;
  MGL_CORE_ASSERT(tex != nil, "[TextureCube] Metal texture invalid.");

  size_t bytesPerPixel = m_components * m_data_type->size;
  size_t rowBytes = m_width * bytesPerPixel;
  rowBytes = ((rowBytes + align - 1) / align) * align;
  size_t expected_size = rowBytes * m_height;
  MGL_CORE_ASSERT(dst.size() >= write_offset + expected_size,
                  "[TextureCube] Destination out of bounds.");

  void *dst_ptr = dst.data() + write_offset;
  MTLRegion region = MTLRegionMake2D(0, 0, m_width, m_height);
  [tex getBytes:dst_ptr
        bytesPerRow:rowBytes
      bytesPerImage:expected_size
         fromRegion:region
        mipmapLevel:0
              slice:face];
}

void texture_cube::write(const mgl::uint8_buffer &src, int face,
                         const mgl::rect &viewport, int align) {
  MGL_CORE_ASSERT(!released(),
                  "[TextureCube] Resource already released or not valid.");
  MGL_CORE_ASSERT(face >= 0 && face <= 5, "[TextureCube] Face must be 0..5.");
  MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                  "[TextureCube] Alignment must be 1, 2, 4 or 8.");

  int x = viewport.x;
  int y = viewport.y;
  int width = viewport.width;
  int height = viewport.height;

  id<MTLTexture> tex = (__bridge id<MTLTexture>)m_handler;
  MGL_CORE_ASSERT(tex != nil, "[TextureCube] Metal texture invalid.");

  size_t bytesPerPixel = m_components * m_data_type->size;
  size_t rowBytes = width * bytesPerPixel;
  rowBytes = ((rowBytes + align - 1) / align) * align;
  size_t expected_size = rowBytes * height;
  MGL_CORE_ASSERT(src.size() >= expected_size,
                  "[TextureCube] Source too small.");

  const void *ptr = src.data();
  MTLRegion region = MTLRegionMake2D(x, y, width, height);
  [tex replaceRegion:region
         mipmapLevel:0
               slice:face
           withBytes:ptr
         bytesPerRow:rowBytes
       bytesPerImage:expected_size];
}

void texture_cube::use(int32_t index) {
  // TODO: Implement texture binding for use
  // This will be handled by the render encoder in Metal
}

} // namespace mgl::metal
