#include "mgl_registry/locations/zip.hpp"
#include "mgl_core/zip.hpp"

namespace mgl::registry
{
  static const std::string zip_location_name = "zip";

  zip_location::zip_location(const mgl::path& path)
      : location(path)
  {
    if(path.is_relative())
      set_path(std::filesystem::current_path() / path);

    if(!std::filesystem::exists(this->path()))
    {
      set_path("");
    }
  }

  istream_ref zip_location::open(const std::string& path, std::ios_base::openmode mode)
  {
    if(is_null())
      return nullptr;

    return mgl::create_ref<mgl::zip_ifstream>(this->path().string(), path);
  }

  bool zip_location::exists(const std::string& path) const
  {
    if(is_null())
      return false;

    zip_file zip(this->path().string());
    return zip.exists(path);
  }

  bool zip_location::can_handle(const url& url) const
  {
    if(url.protocol != "file")
      return false;

    auto path = url.path;

    if(path.empty())
      return false;

    if(path.is_relative())
      path = std::filesystem::current_path() / url.path;

    return std::filesystem::is_regular_file(path) && path.extension() == ".zip";
  }

  location_ref zip_location::factory(const url& url) const
  {
    if(!can_handle(url))
      return nullptr;

    return mgl::ref<location>(new zip_location(url.path));
  }

  bool zip_location::operator==(const location& other) const
  {
    return path() == other.path();
  }

  const std::string& zip_location::name() const
  {
    return zip_location_name;
  }

} // namespace mgl::registry