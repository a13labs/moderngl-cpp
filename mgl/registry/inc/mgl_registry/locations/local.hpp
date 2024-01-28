#pragma once

#include "mgl_core/io.hpp"
#include "mgl_registry/location.hpp"

namespace mgl::registry
{
  class local_location : public location, public location_factory
  {
public:
    local_location()
        : location(std::string())
    { }

    virtual io::istream_ref open_read(const std::string& path,
                                      io::openmode mode = io::binary) override final;

    virtual io::ostream_ref open_write(const std::string& path,
                                       io::openmode mode = io::binary) override final;

    virtual void read(const std::string& path, mgl::uint8_buffer& buffer) const override final;

    virtual void write(const std::string& path,
                       const mgl::uint8_buffer& buffer) const override final;

    virtual bool exists(const std::string& path) const override final;

    virtual bool can_handle(const url& url) const override final;

    virtual location_ref factory(const url& url) const override final;

    virtual const std::string& kind() const override final;

protected:
    local_location(const std::string& path)
        : local_location(mgl::path(path))
    { }

    local_location(const mgl::path& path);
  };
} // namespace mgl::registry