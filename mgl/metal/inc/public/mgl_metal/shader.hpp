#pragma once

#include "mgl_core/containers.hpp"
#include "mgl_core/string.hpp"

namespace mgl::metal
{
  using shader_defines = mgl::dict<std::string, std::string>;

  extern const std::string no_shader;

  // Note: Metal uses entry-point functions inside a single MSL source / library.
  // This class stores source + preferred entry names for stages. Actual compilation
  // to MTLLibrary / MTLFunction is implemented in the .mm file.
  class shader
  {
public:
    enum type
    {
      VERTEX_SHADER,
      FRAGMENT_SHADER,
      COMPUTE_SHADER,

      // Keep GENERIC_PROGRAM for convenience; geometry/tessellation are not
      // first-class in Metal like in GL (tessellation is supported on newer HW).
      GENERIC_PROGRAM
    };

    // source is the full MSL source (can contain multiple entry functions).
    // entry_names map stage -> function name in the MSL source.
    shader(const std::string& source, shader::type type = shader::type::GENERIC_PROGRAM);

    ~shader() = default;

    // Return the full MSL source (optionally can inject defines).
    const std::string source(const shader_defines& defines = {});

    // Return the preferred entry point name for a stage (or empty if none).
    const std::string entry_name(shader::type stage) const;

    // Convenience accessors
    const std::string vertex_entry(const shader_defines& defines = {});
    const std::string fragment_entry(const shader_defines& defines = {});
    const std::string compute_entry(const shader_defines& defines = {});

    // Inspect outputs / attributes can be parsed from source if needed.
    const mgl::string_list outputs();

private:
    std::string m_source;
    int32_t m_version = 0; // optional MSL/SDK version hint
    shader::type m_type;
    mgl::string_list m_attributes;
    mgl::dict<type, std::string> m_entry_names;
  };

  struct shaders
  {
    // For Metal we typically need at most vertex + fragment, or a compute entry.
    // Keep a similar container for convenience.
    const mgl::string_list sources;

    shaders(const std::string& vs_source,
            const std::string& fs_source,
            const std::string& cs_source = "")
        : sources{ vs_source, fs_source, cs_source }
    { }

    shaders(const mgl::string_list& sources)
        : sources{ sources }
    { }

    inline bool empty() { return mgl::all_empty(sources); }
    inline const std::string& vertex() { return sources[0]; }
    inline const std::string& fragment() { return sources[1]; }
    inline const std::string& compute() { return sources[2]; }
  };

} // namespace mgl::metal