#include "mgl_registry/resources/shader.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_core/io.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"
#include "mgl_core/zip.hpp"
#include <regex>

namespace mgl::registry
{

  const std::string no_shader = "";

  static mgl::string_list s_shaders_text = {
    "VERTEX_SHADER",       "FRAGMENT_SHADER",        "GEOMETRY_SHADER",
    "TESS_CONTROL_SHADER", "TESS_EVALUATION_SHADER",
  };

  shader::shader(const std::string& source, shader::type type)
  {
    auto src = mgl::trim(source);
    auto lines = mgl::split(src, '\n');
    MGL_CORE_ASSERT(mgl::starts_with(lines[0], "#version"),
                    "#version must appear on the first line");

    m_type = type;
    m_version_code = mgl::to_int(lines[0].substr(sizeof("#version")));
    m_source = mgl::join('\n', lines, 1);
  }

  bool shader::handle_includes(const location_ref& location, int depth, int source_id)
  {
    if(depth > 10)
    {
      MGL_CORE_ERROR("Too many includes in shader source");
      return false;
    }

    std::string source = m_source;
    std::string include = "#include";
    std::string::size_type pos = source.find(include);

    while(pos != std::string::npos)
    {
      std::string::size_type start = source.find("\"", pos + include.size());
      std::string::size_type end = source.find("\"", start + 1);

      if(start == std::string::npos || end == std::string::npos)
      {
        MGL_CORE_ERROR("Invalid include directive in shader source");
        return false;
      }

      std::string include_path = source.substr(start + 1, end - start - 1);

      mgl::istream_ref file = location->open(include_path, mgl::input_file::in);

      if(!file)
      {
        MGL_CORE_ERROR("Failed to read include file: {}", include_path);
        return false;
      }

      std::string include_source((std::istreambuf_iterator<char>(*file)),
                                 std::istreambuf_iterator<char>());

      if(include_source.empty())
      {
        MGL_CORE_ERROR("Failed to read include file: {}", include_path);
        return false;
      }

      std::string include_directive = source.substr(pos, end - pos + 1);
      source.replace(pos, end - pos + 1, include_source);

      mgl::string_list lines = mgl::split(include_source, '\n');

      for(const std::string& line : lines)
      {
        if(line.find(include) != std::string::npos)
        {
          if(!handle_includes(location, depth + 1, source_id))
          {
            return false;
          }
        }
      }

      pos = source.find(include, pos + include_source.size());
    }

    m_source = source;
    return true;
  }

  const std::string shader::vertex(const shader_defines& defines)
  {
    if(m_type != shader::type::VERTEX_SHADER && m_type != shader::type::GENERIC_PROGRAM)
    {
      return "";
    }
    return source(shader::type::VERTEX_SHADER, defines);
  }

  const std::string shader::fragment(const shader_defines& defines)
  {
    if(m_type != shader::type::FRAGMENT_SHADER && m_type != shader::type::GENERIC_PROGRAM)
    {
      return "";
    }
    return source(shader::type::FRAGMENT_SHADER, defines);
  }

  const std::string shader::geometry(const shader_defines& defines)
  {
    if(m_type != shader::type::GEOMETRY_SHADER && m_type != shader::type::GENERIC_PROGRAM)
    {
      return "";
    }
    return source(shader::type::GEOMETRY_SHADER, defines);
  }

  const std::string shader::tess_control(const shader_defines& defines)
  {
    if(m_type != shader::type::TESS_CONTROL_SHADER && m_type != shader::type::GENERIC_PROGRAM)
    {
      return "";
    }
    return source(shader::type::TESS_CONTROL_SHADER, defines);
  }

  const std::string shader::tess_evaluation(const shader_defines& defines)
  {
    if(m_type != shader::type::TESS_EVALUATION_SHADER && m_type != shader::type::GENERIC_PROGRAM)
    {
      return "";
    }
    return source(shader::type::TESS_EVALUATION_SHADER, defines);
  }

  const std::string shader::source(shader::type type, const shader_defines& defines)
  {
    if(!mgl::in(s_shaders_text[type], m_source))
    {
      if(type == m_type)
        return source(defines);

      return "";
    }

    mgl::string_list str_defines;

    for(const auto& [key, value] : defines)
    {
      str_defines.push_back(std::format("#define {} {}", key, value));
    }

    auto line = (int)str_defines.size() + 2;

    return std::format("#version {}\n#define {}\n{}\n#line {}\n{}",
                       m_version_code,
                       s_shaders_text[type],
                       mgl::join('\n', str_defines),
                       line,
                       m_source);
  }

  const std::string shader::source(const shader_defines& defines)
  {
    mgl::string_list str_defines;

    for(const auto& [key, value] : defines)
    {
      str_defines.push_back(std::format("#define {} {}", key, value));
    }

    auto line = (int)str_defines.size() + 1;

    return std::format("#version {}\n{}\n#line {}\n{}",
                       m_version_code,
                       mgl::join('\n', str_defines),
                       line,
                       m_source);
  }

} // namespace mgl::registry