
#include "mgl_opengl/shader.hpp"
#include <regex>

#include "mgl_core/debug.hpp"

#include <format>

namespace mgl::opengl
{
  const std::string no_shader = "";

  static mgl::string_list s_shaders_text = {
    "VERTEX_SHADER",       "FRAGMENT_SHADER",        "GEOMETRY_SHADER",
    "TESS_CONTROL_SHADER", "TESS_EVALUATION_SHADER",
  };

  static const std::regex
      s_layout_regex("(layout\\(.+\\)\\))?(\\s+)?(out)(\\s+)(\\w+)(\\s+)(\\w+)");

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

  const mgl::string_list shader::outputs()
  {
    mgl::string_list outputs = {};

    std::regex_iterator<std::string::iterator> it(m_source.begin(), m_source.end(), s_layout_regex);
    std::regex_iterator<std::string::iterator> end;

    for(; it != end; ++it)
    {
      auto idx = it->size() - 1;
      auto m = *it;
      outputs.push_back(m[idx].str());
    }

    return outputs;
  }

  void shader::handle_includes(int depth, int source_id) { }

} // namespace  mgl::opengl