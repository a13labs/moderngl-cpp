#include "mgl_metal/data_type.hpp"

#import <Metal/Metal.h>

#include <cstddef>
#include <cstdint>

namespace mgl::metal {

static int float_base_format[5] = {
    0, MTLPixelFormatR32Float, MTLPixelFormatRG32Float,
    MTLPixelFormatRGBA32Float, MTLPixelFormatRGBA32Float};
static int int_base_format[5] = {
    0, MTLPixelFormatR32Sint, MTLPixelFormatRG32Sint, MTLPixelFormatRGBA32Sint,
    MTLPixelFormatRGBA32Sint};

static int f1_internal_format[5] = {
    MTLPixelFormatInvalid, MTLPixelFormatR8Unorm, MTLPixelFormatRG8Unorm,
    MTLPixelFormatRGBA8Unorm, // pad RGB->RGBA
    MTLPixelFormatRGBA8Unorm};

static int f2_internal_format[5] = {
    MTLPixelFormatInvalid, MTLPixelFormatR16Float, MTLPixelFormatRG16Float,
    MTLPixelFormatRGBA16Float, MTLPixelFormatRGBA16Float};

static int f4_internal_format[5] = {
    MTLPixelFormatInvalid, MTLPixelFormatR32Float, MTLPixelFormatRG32Float,
    MTLPixelFormatRGBA32Float, MTLPixelFormatRGBA32Float};

static int u1_internal_format[5] = {
    MTLPixelFormatInvalid, MTLPixelFormatR8Uint, MTLPixelFormatRG8Uint,
    MTLPixelFormatRGBA8Uint, MTLPixelFormatRGBA8Uint};

static int u2_internal_format[5] = {
    MTLPixelFormatInvalid, MTLPixelFormatR16Uint, MTLPixelFormatRG16Uint,
    MTLPixelFormatRGBA16Uint, MTLPixelFormatRGBA16Uint};

static int u4_internal_format[5] = {
    MTLPixelFormatInvalid, MTLPixelFormatR32Uint, MTLPixelFormatRG32Uint,
    MTLPixelFormatRGBA32Uint, MTLPixelFormatRGBA32Uint};

static int i1_internal_format[5] = {
    MTLPixelFormatInvalid, MTLPixelFormatR8Sint, MTLPixelFormatRG8Sint,
    MTLPixelFormatRGBA8Sint, MTLPixelFormatRGBA8Sint};

static int i2_internal_format[5] = {
    MTLPixelFormatInvalid, MTLPixelFormatR16Sint, MTLPixelFormatRG16Sint,
    MTLPixelFormatRGBA16Sint, MTLPixelFormatRGBA16Sint};

static int i4_internal_format[5] = {
    MTLPixelFormatInvalid, MTLPixelFormatR32Sint, MTLPixelFormatRG32Sint,
    MTLPixelFormatRGBA32Sint, MTLPixelFormatRGBA32Sint};

// normalized (n*) variants — map to Unorm / Snorm where appropriate
static int nu1_internal_format[5] = {
    MTLPixelFormatInvalid, MTLPixelFormatR8Unorm, MTLPixelFormatRG8Unorm,
    MTLPixelFormatRGBA8Unorm, MTLPixelFormatRGBA8Unorm};

static int nu2_internal_format[5] = {
    MTLPixelFormatInvalid, MTLPixelFormatR16Unorm, MTLPixelFormatRG16Unorm,
    MTLPixelFormatRGBA16Unorm, MTLPixelFormatRGBA16Unorm};

static int ni1_internal_format[5] = {
    MTLPixelFormatInvalid, MTLPixelFormatR8Snorm, MTLPixelFormatRG8Snorm,
    MTLPixelFormatRGBA8Snorm, MTLPixelFormatRGBA8Snorm};

static int ni2_internal_format[5] = {
    MTLPixelFormatInvalid, MTLPixelFormatR16Snorm, MTLPixelFormatRG16Snorm,
    MTLPixelFormatRGBA16Snorm, MTLPixelFormatRGBA16Snorm};

static data_type f1 = {float_base_format, f1_internal_format,
                       MTLPixelFormatR8Unorm, 1, true};
static data_type f2 = {float_base_format, f2_internal_format,
                       MTLPixelFormatR16Float, 2, true};
static data_type f4 = {int_base_format, f4_internal_format,
                       MTLPixelFormatR32Float, 4, false};
static data_type u1 = {int_base_format, u1_internal_format,
                       MTLPixelFormatR8Uint, 1, false};
static data_type u2 = {int_base_format, u2_internal_format,
                       MTLPixelFormatR16Uint, 2, false};
static data_type u4 = {int_base_format, u4_internal_format,
                       MTLPixelFormatR32Uint, 4, false};
static data_type i1 = {int_base_format, i1_internal_format,
                       MTLPixelFormatR8Sint, 1, false};
static data_type i2 = {int_base_format, i2_internal_format,
                       MTLPixelFormatR16Sint, 2, false};
static data_type i4 = {int_base_format, i4_internal_format,
                       MTLPixelFormatR32Sint, 4, false};

static data_type nu1 = {float_base_format, nu1_internal_format,
                        MTLPixelFormatR8Unorm, 1, false};
static data_type nu2 = {float_base_format, nu2_internal_format,
                        MTLPixelFormatR16Unorm, 2, false};
static data_type ni1 = {float_base_format, ni1_internal_format,
                        MTLPixelFormatR8Snorm, 1, false};
static data_type ni2 = {float_base_format, ni2_internal_format,
                        MTLPixelFormatR16Snorm, 2, false};

// Returns pointer to a static data_type or nullptr if unknown.
data_type *from_dtype(const char *dtype, size_t size) {
  if (size < 2 || size > 3)
    return nullptr;

  if (size == 2) {
    switch (dtype[0] * 256 + dtype[1]) {
    case ('f' * 256 + '1'):
      return &f1;
    case ('f' * 256 + '2'):
      return &f2;
    case ('f' * 256 + '4'):
      return &f4;

    case ('u' * 256 + '1'):
      return &u1;
    case ('u' * 256 + '2'):
      return &u2;
    case ('u' * 256 + '4'):
      return &u4;

    case ('i' * 256 + '1'):
      return &i1;
    case ('i' * 256 + '2'):
      return &i2;
    case ('i' * 256 + '4'):
      return &i4;

    default:
      return nullptr;
    }
  } else // size == 3
  {
    switch (dtype[0] * 65536 + dtype[1] * 256 + dtype[2]) {
    case ('n' * 65536 + 'i' * 256 + '1'):
      return &ni1;
    case ('n' * 65536 + 'i' * 256 + '2'):
      return &ni2;
    case ('n' * 65536 + 'u' * 256 + '1'):
      return &nu1;
    case ('n' * 65536 + 'u' * 256 + '2'):
      return &nu2;
    default:
      return nullptr;
    }
  }
}

} // namespace mgl::metal