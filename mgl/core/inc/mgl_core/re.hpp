/**
 * @file re.hpp
 * @brief Contains the definition of the re namespace and related types for core regular expression functionality.

*/
#pragma once
#include "string.hpp"

namespace mgl
{
  /**
  * Splits a string into a list of substrings based on a regular expression.
  * 
  * @param input The input string to be split.
  * @param re The regular expression used for splitting the string.
  * @return A list of substrings obtained by splitting the input string.
  */
  string_list re_split(const std::string& input, std::string re);
}