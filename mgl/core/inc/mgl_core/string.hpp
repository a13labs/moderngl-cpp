#pragma once
#include "platform.hpp"
#include "containers.hpp"
#include "memory.hpp"

#include <functional>
#include <sstream>
#include <string>

#ifdef MGL_PLATFORM_MACOS
#include <algorithm>
#endif

namespace mgl
{
  using string_list = list<std::string>;

  static const size_t npos = std::string::npos;

  string_list split(const std::string& s, char delimiter);

  std::string trim(std::string s);
  std::string trim(std::string s, const std::function<bool(char)>& predicate);

  std::string join(char delimiter,
                   const std::vector<std::string>& vec,
                   std::size_t start_index = 0,
                   std::size_t end_index = npos);

  template <typename T>
  std::string join(char delimiter,
                   const list<T>& vec,
                   std::size_t start_index = 0,
                   std::size_t end_index = npos)
  {
    std::ostringstream result;

    if(end_index == npos)
      end_index = vec.size() - 1;
    else if(end_index > vec.size() - 1)
      return "";

    if(end_index < start_index || start_index >= vec.size())
      return "";

    for(std::size_t i = start_index; i <= end_index; ++i)
    {
      if(i > start_index)
        result << delimiter;
      result << vec[i];
    }

    return result.str();
  }

  inline bool in(const std::string& e, const std::string& str)
  {
    return str.find(e) != npos;
  }

  inline bool in(const std::string& e, const string_list& l)
  {
    return !(std::find(l.begin(), l.end(), e) == l.end());
  }

  inline bool starts_with(const std::string& str, std::string e)
  {
    return str.rfind(e, 0) == 0;
  }

  inline int to_int(const std::string& str)
  {
    return std::stoi(str);
  };

  inline bool all_empty(const string_list& lst)
  {
    return std::all_of(lst.begin(), lst.end(), [](const std::string& str) { return str.empty(); });
  }

} // namespace  mgl