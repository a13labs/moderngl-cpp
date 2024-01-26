#include "mgl_registry/loaders/text.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_core/zip.hpp"

namespace mgl::registry::loaders
{
  static text_loader_options default_text_loader_options;

  mgl::string_list text_loader::get_extensions() const
  {
    return { ".txt" };
  }

  resource_ref text_loader::load(const location_ref& location,
                                 const std::string& path,
                                 const loader_options& options)
  {
    const text_loader_options* opts = dynamic_cast<const text_loader_options*>(&options);

    if(!opts)
    {
      opts = &default_text_loader_options;
    }

    mgl::istream_ref file = location->open_read(path);

    if(!file)
    {
      MGL_CORE_ERROR("Failed to read text file: {}", path);
      return nullptr;
    }

    std::string src((std::istreambuf_iterator<char>(*file)), std::istreambuf_iterator<char>());
    return mgl::create_ref<text>(src);
  }

} // namespace mgl::registry::loaders