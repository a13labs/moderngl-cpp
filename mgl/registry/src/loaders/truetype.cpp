

#include "mgl_registry/loaders/truetype.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_core/math.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_registry/resources/fonts/truetype.hpp"

namespace mgl::registry::loaders
{
  // The default font loader options. ASCII characters from 0x20 to 0xFF and a pixel height of 96.
  static const font_loader_options default_font_loader_options;

  mgl::string_list font_loader::get_extensions() const
  {
    return { ".ttf", ".otf" };
  }

  resource_ref font_loader::load(const location_ref& location,
                                 const std::string& path,
                                 const loader_options& options)
  {
    if(!location->exists(path))
    {
      MGL_CORE_ERROR("Failed to load font: {0}", path);
      return nullptr;
    }

    const font_loader_options* opts = dynamic_cast<const font_loader_options*>(&options);

    if(!opts)
    {
      opts = &default_font_loader_options;
    }

    uint8_buffer buffer;

    // Read the font file into a buffer and check if it's a valid font file.
    location->read(path, buffer);

    return mgl::create_ref<truetype_font>(buffer);
  }

} // namespace mgl::registry::loaders
