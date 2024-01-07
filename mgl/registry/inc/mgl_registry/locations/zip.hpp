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

    virtual ifsteam_ptr open(const std::string& path,
                             std::ios_base::openmode mode = std::ios_base::in) override final;

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