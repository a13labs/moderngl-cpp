/*
   This file contains container templates used in the mgl namespace.
   The containers include list, pair, dict, and unordered_map.
   These templates are aliases for the corresponding containers in the standard library.
   They are used to provide a more concise and readable syntax in the mgl namespace.
   The containers are defined as follows:

   - list<T>: Alias for std::vector<T>, representing a dynamic array.
   - pair<T, U>: Alias for std::pair<T, U>, representing a pair of values.
   - dict<T, U>: Alias for std::map<T, U>, representing an associative container with key-value pairs.
   - unordered_map<T, U>: Alias for std::unordered_map<T, U>, representing an unordered associative container with key-value pairs.

   These containers are commonly used in the mgl library for data storage and manipulation.
   They provide a convenient and efficient way to work with collections of objects.
*/
#pragma once

#include <map>
#include <unordered_map>
#include <vector>

/**
 * @brief Namespace mgl contains aliases for various container types.
 */
namespace mgl
{
  /**
   * @brief Alias for std::vector<T>.
   * @tparam T The type of elements in the list.
   */
  template <typename T>
  using list = std::vector<T>;

  /**
   * @brief Alias for std::pair<T, U>.
   * @tparam T The type of the first element in the pair.
   * @tparam U The type of the second element in the pair.
   */
  template <typename T, typename U>
  using pair = std::pair<T, U>;

  /**
   * @brief Alias for std::map<T, U>.
   * @tparam T The type of keys in the dictionary.
   * @tparam U The type of values in the dictionary.
   */
  template <typename T, typename U>
  using dict = std::map<T, U>;

  /**
   * @brief Alias for std::unordered_map<T, U>.
   * @tparam T The type of keys in the unordered map.
   * @tparam U The type of values in the unordered map.
   */
  template <typename T, typename U>
  using unordered_map = std::unordered_map<T, U>;
} // namespace mgl
