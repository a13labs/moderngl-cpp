#pragma once

#include "mgl_core/io.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"

namespace mgl::registry
{
  struct location
  {
    std::string path;
    bool is_compressed;
    bool is_remote;

    location(const std::string& path, bool is_compressed, bool is_remote = false)
        : path(path)
        , is_compressed(is_compressed)
        , is_remote(is_remote)
    { }

    location(const mgl::path& path, bool is_compressed, bool is_remote = false)
        : path(path.string())
        , is_compressed(is_compressed)
        , is_remote(is_remote)
    { }

    location(const location& other)
        : path(other.path)
        , is_compressed(other.is_compressed)
        , is_remote(other.is_remote)
    { }

    bool operator==(const location& other) const
    {
      return path == other.path && is_compressed == other.is_compressed &&
             is_remote == other.is_remote;
    }

    bool is_null() const { return path == mgl::null_path; }
  };

  using locations = mgl::list<location>;

  extern location null_location;

} // namespace mgl::registry