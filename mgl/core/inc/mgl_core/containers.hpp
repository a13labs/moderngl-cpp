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
