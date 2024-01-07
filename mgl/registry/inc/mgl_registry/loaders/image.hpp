#pragma once

#include "mgl_registry/loader.hpp"
#include "mgl_registry/resources/image.hpp"

namespace mgl::registry::loaders
{
  class image_loader;
  using image_loader_ref = mgl::scope<image_loader>;

  struct image_loader_options : public loader_options
  {
    bool flip_vertically = false;
    bool flip_horizontally = false;

    virtual ~image_loader_options() = default;
  };

  class image_loader : public loader
  {
public:
    image_loader() = default;
    virtual ~image_loader() = default;

    virtual resource::type get_type() const override { return resource::type::image; }

    virtual mgl::string_list get_extensions() const override;

    virtual resource_ref load(const location_ref& location,
                              const std::string& path,
                              const loader_options& options) override;
  };
} // namespace mgl::registry::loaders