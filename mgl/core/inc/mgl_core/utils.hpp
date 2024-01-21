/**
 * @brief This file contains utility functions for the mgl::core namespace.
 */

#pragma once

#include "containers.hpp"

#define BIT(x) 1 << x

namespace mgl
{

  /**
   * @brief Zips two lists together into a list of pairs.
   * 
   * @tparam T The type of elements in the first list.
   * @tparam U The type of elements in the second list.
   * @param a The first list.
   * @param b The second list.
   * @return The zipped list of pairs.
   */
  template <typename T, typename U>
  list<pair<T, U>> zip(const list<T>& a, const list<U>& b)
  {
    list<pair<T, U>> zipped;

    for(std::size_t i = 0; i < a.size(); ++i)
    {
      zipped.emplace_back(a[i], b[i]);
    }

    return zipped;
  }
} // namespace  mgl