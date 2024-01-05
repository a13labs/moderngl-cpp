#pragma once
#include "location.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"
#include "resource.hpp"

namespace mgl::registry
{
  struct loader_options
  {
    virtual ~loader_options() = default;
  };

  class loader
  {
public:
    virtual ~loader() = default;
    virtual resource::type get_type() const = 0;
    virtual mgl::string_list get_extensions() const = 0;
    virtual resource_ref
    load(const location& location, const std::string& path, const loader_options& options) = 0;
  };

  using loader_ref = mgl::scope<loader>;

} // namespace mgl::registry