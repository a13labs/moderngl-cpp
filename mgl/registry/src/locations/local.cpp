#include "mgl_registry/locations/local.hpp"

namespace mgl::registry
{
  static const std::string local_location_name = "local";

  local_location::local_location(const mgl::path& path)
      : location(path)
  {
    if(path.is_relative())
      set_path(std::filesystem::current_path() / path);

    if(!std::filesystem::exists(this->path()))
    {
      set_path("");
    }
  }

  istream_ref local_location::open(const std::string& path, std::ios_base::openmode mode)
  {
    if(is_null())
      return nullptr;

    return mgl::create_ref<mgl::ifstream>(this->path() / path, mode);
  }

  bool local_location::exists(const std::string& path) const
  {
    if(is_null())
      return false;

    return std::filesystem::exists(this->path() / path);
  }

  bool local_location::can_handle(const url& url) const
  {
    if(url.protocol != "file")
      return false;

    auto path = url.path;

    if(path.empty())
      return false;

    if(path.is_relative())
      path = std::filesystem::current_path() / url.path;

    return std::filesystem::is_directory(url.path);
  }

  location_ref local_location::factory(const url& url) const
  {
    if(!can_handle(url))
      return nullptr;

    return mgl::ref<location>(new local_location(url.path));
  }

  const std::string& local_location::name() const
  {
    return local_location_name;
  }

  bool local_location::operator==(const location& other) const
  {
    return path() == other.path();
  }
} // namespace mgl::registry