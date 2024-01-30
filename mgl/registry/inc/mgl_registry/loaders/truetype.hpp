#pragma once

#include "mgl_registry/loader.hpp"
#include "mgl_registry/resources/font.hpp"

namespace mgl::registry::loaders
{
  class font_loader;

  struct font_loader_options : public loader_options
  {
    virtual ~font_loader_options() = default;
  };

  class font_loader : public loader
  {
public:
    font_loader() = default;
    virtual ~font_loader() = default;

    virtual resource::type get_type() const override { return resource::type::font; }

    virtual mgl::string_list get_extensions() const override;

    virtual resource_ref load(const location_ref& location,
                              const std::string& path,
                              const loader_options& options) override;
  };

  using font_loader_ref = mgl::scope<font_loader>;
} // namespace mgl::registry::loaders