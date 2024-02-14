#pragma once

#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"
#include "mgl_registry/location.hpp"
#include "mgl_registry/resource.hpp"

namespace mgl::registry
{
  class shader;
  using shader_ref = mgl::ref<shader>;
  using shader_defines = mgl::dict<std::string, std::string>;
  using shaders_outputs = mgl::string_list;

  class shader : public resource
  {
    friend class shader_loader;

public:
    enum type
    {
      VERTEX_SHADER,
      FRAGMENT_SHADER,
      GEOMETRY_SHADER,
      TESS_CONTROL_SHADER,
      TESS_EVALUATION_SHADER,

      GENERIC_PROGRAM
    };

    shader(const std::string& source, shader::type type);
    ~shader() = default;

    shader(const shader&) = delete;
    shader(shader&&) = delete;
    shader& operator=(const shader&) = delete;
    shader& operator=(shader&&) = delete;

    virtual resource::type get_type() const override { return resource::type::shader; }

    const std::string source(shader::type type, const shader_defines& defines = {});
    const std::string source(const shader_defines& defines = {});

    const mgl::string_list outputs();

    const std::string vertex(const shader_defines& defines = {});
    const std::string fragment(const shader_defines& defines = {});
    const std::string geometry(const shader_defines& defines = {});
    const std::string tess_control(const shader_defines& defines = {});
    const std::string tess_evaluation(const shader_defines& defines = {});

private:
    bool handle_includes(const location_ref& location, int depth = 0, int source_id = 0);

    std::string m_source;
    int m_version;
    shader::type m_type;
    mgl::string_list m_attributes;
  };

} // namespace mgl::registry