#include "mgl_registry/loaders/shader.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_core/io.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/zip.hpp"
#include "mgl_registry/registry.hpp"

#include <regex>

namespace mgl::registry::loaders
{
  static shader_loader_options default_shader_loader_options;

  const std::string no_shader = "";

  static mgl::string_list s_shaders_text = {
    "VERTEX_SHADER",       "FRAGMENT_SHADER",        "GEOMETRY_SHADER",
    "TESS_CONTROL_SHADER", "TESS_EVALUATION_SHADER",
  };

  mgl::string_list shader_loader::get_extensions() const
  {
    return { ".vs", ".fs", ".gs", ".cs", ".ts", ".glsl" };
  }

  resource_ref shader_loader::load(const location_ref& location,
                                   const std::string& path,
                                   const loader_options& options)
  {
    const shader_loader_options* opts = dynamic_cast<const shader_loader_options*>(&options);

    if(!opts)
    {
      opts = &default_shader_loader_options;
    }

    mgl::io::istream_ref file = location->open_read(path);

    if(!file)
    {
      MGL_CORE_ERROR("Failed to read shader file: {}", path);
      return nullptr;
    }

    std::string src((std::istreambuf_iterator<char>(*file)), std::istreambuf_iterator<char>());
    return mgl::create_ref<shader>(src, opts->type);
  }

} // namespace mgl::registry::loaders