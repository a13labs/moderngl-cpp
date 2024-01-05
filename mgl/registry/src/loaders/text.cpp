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

  resource_ref text_loader::load(const location& location,
                                 const std::string& path,
                                 const loader_options& options)
  {
    const text_loader_options* opts = dynamic_cast<const text_loader_options*>(&options);

    if(!opts)
    {
      opts = &default_text_loader_options;
    }

    std::string src = "";
    if(location.is_compressed)
    {
      mgl::zip_file zip(location.path);
      if(!zip.exists(path))
      {
        return nullptr;
      }
      mgl::buffer<uint8_t> buffer;
      zip.read(path, buffer);
      src = std::string(buffer.begin(), buffer.end());
    }
    else
    {
      auto full_path = mgl::path(location.path) / path;
      if(!std::filesystem::exists(full_path))
      {
        MGL_CORE_ERROR("Failed to read image file: {}", path);
        return nullptr;
      }

      mgl::input_file file(full_path, mgl::input_file::in);
      src = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    }

    return mgl::create_ref<text>(src);
  }

} // namespace mgl::registry::loaders