#include "mgl_registry/locations/local.hpp"
#include "mgl_core/io.hpp"

namespace mgl::registry
{
  static const std::string local_location_name = "local";

  local_location::local_location(const mgl::path& path)
      : location(path)
  {
    if(path.empty())
      return;

    if(path.is_relative())
      set_path(std::filesystem::current_path() / path);

    if(!std::filesystem::exists(this->path()))
    {
      set_path("");
    }
  }

  io::istream_ref local_location::open_read(const std::string& path, io::openmode mode)
  {
    if(is_null())
      return nullptr;

    return mgl::create_ref<mgl::io::ifstream>(this->path() / path, io::in | mode);
  }

  io::ostream_ref local_location::open_write(const std::string& path, io::openmode mode)
  {
    if(is_null())
      return nullptr;

    return mgl::create_ref<mgl::io::ofstream>(this->path() / path, io::out | mode);
  }

  void local_location::read(const std::string& path, mgl::uint8_buffer& buffer) const
  {
    if(is_null())
      return;

    auto file_size = std::filesystem::file_size(this->path() / path);
    buffer.resize(file_size);
    auto stream = mgl::io::open_read(this->path() / path);
    io::read_uint8_buffer(stream, buffer);
    stream->close();
  }

  void local_location::write(const std::string& path, const mgl::uint8_buffer& buffer) const
  {
    if(is_null())
      return;

    auto stream = mgl::io::open_write(this->path() / path);
    io::write_uint8_buffer(stream, buffer);
    stream->flush();
    stream->close();
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

} // namespace mgl::registry