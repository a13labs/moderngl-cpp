#include "mgl_core/string.hpp"
#include "mgl_core/log.hpp"

namespace mgl
{
  string_list split(const std::string& s, char delimiter)
  {
    string_list tokens;
    std::string token;
    std::istringstream token_stream(s);
    while(std::getline(token_stream, token, delimiter))
    {
      if(!token.empty())
      {
        tokens.push_back(token);
      }
    }
    if(!s.empty() && s.back() == delimiter)
    {
      tokens.push_back("");
    }
    return tokens;
  }

  std::string trim(std::string s)
  {
    s.erase(s.begin(), std::find_if_not(s.begin(), s.end(), isspace));
    s.erase(std::find_if_not(s.rbegin(), s.rend(), isspace).base(), s.end());
    return s;
  }

  std::string trim(std::string s, const std::function<bool(char)>& predicate)
  {
    s.erase(s.begin(), std::find_if_not(s.begin(), s.end(), predicate));
    s.erase(std::find_if_not(s.rbegin(), s.rend(), predicate).base(), s.end());
    return s;
  }

  std::string join(char delimiter, const string_list& vec, size_t start_index, size_t end_index)
  {
    if(end_index == npos)
      end_index = vec.size();

    if(end_index < start_index)
      return "";

    std::string ret;

    if(start_index < vec.size() && end_index <= vec.size())
    {
      if(end_index == start_index)
        return vec.at(end_index);

      for(auto it = vec.begin() + start_index; it != vec.begin() + end_index; it++)
      {
        if(!ret.empty())
          ret += delimiter;
        ret += *it;
      }
    }

    return ret;
  }

} // namespace  mgl
