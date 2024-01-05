#pragma once

#include "mgl_registry/loader.hpp"
#include "mgl_registry/resources/shader.hpp"

namespace mgl::registry::loaders
{
  class shader_loader;
  using shader_loader_ref = mgl::scope<shader_loader>;

  struct shader_loader_options : public loader_options
  {
    shader_defines defines = {};
    shaders_outputs outputs = {};
    shader::type type = shader::type::GENERIC_PROGRAM;

    shader_loader_options() = default;
    virtual ~shader_loader_options() = default;
  };

  class shader_loader : public loader
  {
public:
    shader_loader() = default;
    virtual ~shader_loader() = default;

    virtual resource::type get_type() const override { return resource::type::image; }

    virtual mgl::string_list get_extensions() const override;

    virtual resource_ref
    load(const location& location, const std::string& path, const loader_options& options) override;
  };

  using shader_loader_ref = mgl::scope<shader_loader>;
} // namespace mgl::registry::loaders