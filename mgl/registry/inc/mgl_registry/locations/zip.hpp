#pragma once

#include "mgl_core/io.hpp"
#include "mgl_registry/location.hpp"

namespace mgl::registry
{
  class zip_location : public location, public location_factory
  {
public:
    zip_location()
        : location(std::string())
    { }

    virtual istream_ref open_read(const std::string& path,
                                  openmode mode = file_mode::binary) override final;

    virtual ostream_ref open_write(const std::string& path,
                                   openmode mode = file_mode::binary) override final;

    virtual void read(const std::string& path, mgl::uint8_buffer& buffer) const override final;

    virtual void write(const std::string& path,
                       const mgl::uint8_buffer& buffer) const override final;

    virtual bool exists(const std::string& path) const override final;

    virtual bool can_handle(const url& url) const override final;

    virtual location_ref factory(const url& url) const override final;

    virtual const std::string& name() const override final;

    virtual bool operator==(const location& other) const override final;

protected:
    zip_location(const std::string& path)
        : location(std::string())
    { }

    zip_location(const mgl::path& path);
  };
} // namespace mgl::registry