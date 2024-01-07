#pragma once

#include "mgl_registry/loader.hpp"
#include "mgl_registry/resources/text.hpp"

namespace mgl::registry::loaders
{
  class text_loader;
  using text_loader_ref = mgl::scope<text_loader>;

  struct text_loader_options : public loader_options
  {
    text_loader_options() = default;
    virtual ~text_loader_options() = default;
  };

  class text_loader : public loader
  {
public:
    text_loader() = default;
    virtual ~text_loader() = default;

    virtual resource::type get_type() const override { return resource::type::image; }

    virtual mgl::string_list get_extensions() const override;

    virtual resource_ref load(const location_ref& location,
                              const std::string& path,
                              const loader_options& options) override;
  };
} // namespace mgl::registry::loaders