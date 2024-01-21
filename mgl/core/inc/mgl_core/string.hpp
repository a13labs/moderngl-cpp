/**
 * @file string.hpp
 * @brief This file contains the declaration of string manipulation functions and utilities.
 */

#pragma once
#include "containers.hpp"
#include "memory.hpp"
#include "platform.hpp"

#include <functional>
#include <sstream>
#include <string>

#ifdef MGL_PLATFORM_MACOS
#  include <algorithm>
#endif

namespace mgl
{
  using string_list = list<std::string>;

  static const size_t npos = std::string::npos;

  /**
   * @brief Splits a string into a list of substrings based on a delimiter.
   * @param s The input string to be split.
   * @param delimiter The character used as the delimiter.
   * @return A list of substrings.
   */
  string_list split(const std::string& s, char delimiter);

  /**
   * @brief Trims leading and trailing whitespace from a string.
   * @param s The input string to be trimmed.
   * @return The trimmed string.
   */
  std::string trim(std::string s);

  /**
   * @brief Trims leading and trailing characters from a string based on a predicate function.
   * @param s The input string to be trimmed.
   * @param predicate The predicate function that determines which characters to trim.
   * @return The trimmed string.
   */
  std::string trim(std::string s, const std::function<bool(char)>& predicate);

  /**
   * @brief Joins a vector of strings into a single string, using a delimiter.
   * @param delimiter The character used as the delimiter.
   * @param vec The vector of strings to be joined.
   * @param start_index The starting index of the vector to be joined (inclusive).
   * @param end_index The ending index of the vector to be joined (inclusive).
   * @return The joined string.
   */
  std::string join(char delimiter,
                   const std::vector<std::string>& vec,
                   std::size_t start_index = 0,
                   std::size_t end_index = npos);

  /**
   * @brief Joins a list of elements into a single string, using a delimiter.
   * @tparam T The type of elements in the list.
   * @param delimiter The character used as the delimiter.
   * @param vec The list of elements to be joined.
   * @param start_index The starting index of the list to be joined (inclusive).
   * @param end_index The ending index of the list to be joined (inclusive).
   * @return The joined string.
   */
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

  /**
   * @brief Checks if a substring is present in a string.
   * @param e The substring to search for.
   * @param str The string to search in.
   * @return True if the substring is found, false otherwise.
   */
  inline bool in(const std::string& e, const std::string& str)
  {
    return str.find(e) != npos;
  }

  /**
   * @brief Checks if an element is present in a list of strings.
   * @param e The element to search for.
   * @param l The list of strings to search in.
   * @return True if the element is found, false otherwise.
   */
  inline bool in(const std::string& e, const string_list& l)
  {
    return !(std::find(l.begin(), l.end(), e) == l.end());
  }

  /**
   * @brief Checks if a string starts with a specific substring.
   * @param str The string to check.
   * @param e The substring to check for.
   * @return True if the string starts with the substring, false otherwise.
   */
  inline bool starts_with(const std::string& str, std::string e)
  {
    return str.rfind(e, 0) == 0;
  }

  /**
   * @brief Converts a string to an integer.
   * @param str The string to convert.
   * @return The converted integer value.
   */
  inline int to_int(const std::string& str)
  {
    return std::stoi(str);
  };

  /**
   * @brief Checks if all strings in a list are empty.
   * @param lst The list of strings to check.
   * @return True if all strings are empty, false otherwise.
   */
  inline bool all_empty(const string_list& lst)
  {
    return std::all_of(lst.begin(), lst.end(), [](const std::string& str) { return str.empty(); });
  }

} // namespace  mgl