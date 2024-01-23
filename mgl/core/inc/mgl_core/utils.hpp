/**
 * @brief This file contains utility functions for the mgl::core namespace.
 */

#pragma once

#include "containers.hpp"
#include <initializer_list>
#include <utility>

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

  /**
     * @brief Hash function for generic data
     * 
     * @param data data to hash
     * @param size size of data 
     */
  size_t hash_func(const void* data, size_t size);

  /**
     * @brief Creates a new string using printf format
     * 
     * @param format format
     * @param args args
     * @return string& 
     */
  char* sprintf(const char* format, ...);

  // Generic implementation
  template <typename K>
  struct hash
  {
    static size_t get(const K& k) { return hash_func(&k, sizeof(K)); }
  };
} // namespace mgl