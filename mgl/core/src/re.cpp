#include "mgl_core/re.hpp"
#include <regex>

namespace mgl
{
  string_list re_split(const std::string& input, std::string re)
  {
    // The regular expression to use for splitting
    std::regex regex(re); // Split on one or more whitespace characters

    // Split the input string and store the resulting substrings in a vector
    std::vector<std::string> tokens;
    std::copy(std::sregex_token_iterator(input.begin(), input.end(), regex, -1),
              std::sregex_token_iterator(),
              std::back_inserter(tokens));

    return tokens;
  }
} // namespace  mgl