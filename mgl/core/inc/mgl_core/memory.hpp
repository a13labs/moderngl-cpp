/**
 * @file memory.hpp
 * @brief This file contains the definition of various memory-related utilities.
 */

#pragma once

#include <memory>
#include <queue>
#include <vector>

namespace mgl
{
  /**
   * @brief A template alias for a unique pointer.
   * 
   * @tparam T The type of the object to be managed.
   */
  template <typename T>
  using scope = std::unique_ptr<T>;

  /**
   * @brief Creates a unique pointer to an object of type T.
   * 
   * @tparam T The type of the object to be created.
   * @tparam Args The types of the arguments to be passed to the constructor of T.
   * @param args The arguments to be passed to the constructor of T.
   * @return A unique pointer to the created object.
   */
  template <typename T, typename... Args>
  constexpr scope<T> create_scope(Args&&... args)
  {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }

  /**
   * @brief A template alias for a shared pointer.
   * 
   * @tparam T The type of the object to be managed.
   */
  template <typename T>
  using ref = std::shared_ptr<T>;

  /**
   * @brief A vector of shared pointers to objects of type T.
   * 
   * @tparam T The type of the objects in the vector.
   */
  template <typename T>
  using ref_list = std::vector<ref<T>>;

  /**
   * @brief A queue of shared pointers to objects of type T.
   * 
   * @tparam T The type of the objects in the queue.
   */
  template <typename T>
  using ref_queue = std::queue<ref<T>>;

  /**
   * @brief Creates a shared pointer to an object of type T.
   * 
   * @tparam T The type of the object to be created.
   * @tparam Args The types of the arguments to be passed to the constructor of T.
   * @param args The arguments to be passed to the constructor of T.
   * @return A shared pointer to the created object.
   */
  template <typename T, typename... Args>
  constexpr ref<T> create_ref(Args&&... args)
  {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }

  /**
   * @brief A template base class for objects that can create shared pointers to themselves.
   * 
   * @tparam T The derived class type.
   */
  template <typename T>
  using ref_from_this = std::enable_shared_from_this<T>;

  /**
   * @brief A template alias for a vector of elements of type T.
   * 
   * @tparam T The type of the elements in the buffer.
   */
  template <typename T>
  using buffer = std::vector<T>;

  /**
   * @brief A buffer of bytes.
   */
  using byte_buffer = buffer<uint8_t>;

  /**
   * @brief A buffer of floating-point numbers.
   */
  using float_buffer = buffer<float>;

  /**
   * @brief A buffer of integers.
   */
  using int_buffer = buffer<int>;

  /**
   * @brief A buffer of unsigned integers.
   */
  using uint_buffer = buffer<uint32_t>;

} // namespace mgl