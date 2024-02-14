#pragma once

#include "mgl_core/containers.hpp"
#include "mgl_core/string.hpp"

namespace mgl::opengl
{
  using shader_defines = mgl::dict<std::string, std::string>;

  extern const std::string no_shader;

  class shader
  {
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

    shader(const std::string& source, shader::type type = shader::type::GENERIC_PROGRAM);
    ~shader() = default;

    const std::string source(shader::type type, const shader_defines& defines = {});
    const std::string source(const shader_defines& defines);

    const mgl::string_list outputs();

    const std::string vertex(const shader_defines& defines = {});
    const std::string fragment(const shader_defines& defines = {});
    const std::string geometry(const shader_defines& defines = {});
    const std::string tess_control(const shader_defines& defines = {});
    const std::string tess_evaluation(const shader_defines& defines = {});

private:
    std::string m_source;
    int32_t m_version;
    shader::type m_type;
    mgl::string_list m_attributes;
  };

  struct shaders
  {
    const mgl::string_list sources;

    shaders(const std::string& vs_source,
            const std::string& fs_source,
            const std::string& gs_source,
            const std::string& tes_source,
            const std::string& tcs_source)
        : sources{ vs_source, fs_source, gs_source, tes_source, tcs_source }
    { }

    shaders(const std::string& vs_source,
            const std::string& fs_source,
            const std::string& gs_source,
            const std::string& tes_source)
        : sources{ vs_source, fs_source, gs_source, tes_source, no_shader }
    { }

    shaders(const std::string& vs_source,
            const std::string& fs_source,
            const std::string& gs_source)
        : sources{ vs_source, fs_source, gs_source, no_shader, no_shader }
    { }

    shaders(const std::string& vs_source, const std::string& fs_source)
        : sources{ vs_source, fs_source, no_shader, no_shader, no_shader }
    { }

    shaders(const std::string& vs_source)
        : sources{ vs_source, no_shader, no_shader, no_shader, no_shader }
    { }

    shaders(shader& source, const shader_defines& defines = {})
        : sources{ source.vertex(defines),
                   source.fragment(defines),
                   source.geometry(defines),
                   source.tess_control(defines),
                   source.tess_evaluation(defines) }
    { }

    shaders(const mgl::string_list& sources)
        : sources{ sources }
    { }

    shaders(const shaders& other)
        : sources{ other.sources }
    { }

    shaders(shaders&& other)
        : sources{ std::move(other.sources) }
    { }

    inline bool empty() { return mgl::all_empty(sources); }
    inline const std::string& vertex() { return sources[shader::type::VERTEX_SHADER]; }
    inline const std::string& fragment() { return sources[shader::type::FRAGMENT_SHADER]; }
    inline const std::string& geometry() { return sources[shader::type::GEOMETRY_SHADER]; }
    inline const std::string& tess_control() { return sources[shader::type::TESS_CONTROL_SHADER]; }
    inline const std::string& tess_evaluation()
    {
      return sources[shader::type::TESS_EVALUATION_SHADER];
    }
  };

  inline const std::string shader::vertex(const shader_defines& defines)
  {
    return source(shader::type::VERTEX_SHADER, defines);
  }

  inline const std::string shader::fragment(const shader_defines& defines)
  {
    return source(shader::type::FRAGMENT_SHADER, defines);
  }

  inline const std::string shader::geometry(const shader_defines& defines)
  {
    return source(shader::type::GEOMETRY_SHADER, defines);
  }

  inline const std::string shader::tess_control(const shader_defines& defines)
  {
    return source(shader::type::TESS_CONTROL_SHADER, defines);
  }

  inline const std::string shader::tess_evaluation(const shader_defines& defines)
  {
    return source(shader::type::TESS_EVALUATION_SHADER, defines);
  }

} // namespace  mgl::opengl