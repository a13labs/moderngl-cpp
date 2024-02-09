
#include "mgl_opengl/shader.hpp"
#include <regex>

#include "mgl_core/debug.hpp"
#include "mgl_core/io.hpp"

#include <format>

namespace mgl::opengl
{
  const std::string no_shader = "";

  std::string handle_includes(const std::string& source, int32_t depth)
  {
    if(depth > 10)
    {
      MGL_CORE_ERROR("Too many includes");
      return "";
    }

    auto lines = mgl::split(source, '\n');
    mgl::string_list new_lines;

    for(auto&& line : lines)
    {
      if(mgl::starts_with(line, "#include"))
      {
        auto include_file = mgl::trim(line.substr(sizeof("#include")));
        auto file = mgl::io::open_read(include_file);
        auto file_content = mgl::trim(mgl::io::read_text(file));
        new_lines.push_back(handle_includes(file_content, depth + 1));
      }
      else
      {
        new_lines.push_back(line);
      }
    }

    return mgl::join('\n', new_lines);
  }

  shader::shader(const std::string& source, shader::type type)
  {
    auto src = mgl::trim(source);
    src = handle_includes(src, 0);
    auto lines = mgl::split(src, '\n');
    MGL_CORE_ASSERT(mgl::starts_with(lines[0], "#version"),
                    "#version must appear on the first line");

    m_type = type;
    m_version = mgl::to_int(lines[0].substr(sizeof("#version")));
    m_source = mgl::join('\n', lines, 1);
  }

  const std::string shader::source(shader::type type, const shader_defines& defines)
  {
    static mgl::string_list s_shaders_text = {
      "VERTEX_SHADER",       "FRAGMENT_SHADER",        "GEOMETRY_SHADER",
      "TESS_CONTROL_SHADER", "TESS_EVALUATION_SHADER",
    };

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

    auto line = (int32_t)str_defines.size() + 2;

    return std::format("#version {}\n#define {}\n{}\n#line {}\n{}",
                       m_version,
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

    auto line = (int32_t)str_defines.size() + 1;

    return std::format(
        "#version {}\n{}\n#line {}\n{}", m_version, mgl::join('\n', str_defines), line, m_source);
  }

  const mgl::string_list shader::outputs()
  {
    static const std::regex s_layout_regex(
        "(layout\\(.+\\)\\))?(\\s+)?(out)(\\s+)(\\w+)(\\s+)(\\w+)");

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

} // namespace  mgl::opengl