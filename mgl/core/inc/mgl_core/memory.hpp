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
   * @brief A template alias for a weak pointer.
   * 
   * @tparam T The type of the object to be managed.
   */
  template <typename T>
  using weak_ref = std::weak_ptr<T>;

  /**
   * @brief A template alias for a vector of weak pointers to objects of type T.
  */
  template <typename T>
  using weak_ref_list = std::vector<weak_ref<T>>;

  /**
   * @brief A template alias for a queue of weak pointers to objects of type T.
  */
  template <typename T>
  using weak_ref_queue = std::queue<weak_ref<T>>;

  /**
   * @brief Creates a weak pointer to an object of type T.
  */
  template <typename T, typename... Args>
  constexpr weak_ref<T> create_weak_ref(Args&&... args)
  {
    return std::weak_ptr<T>(std::make_shared<T>(std::forward<Args>(args)...));
  }

  /**
   * @brief A template alias for a vector of elements of type T.
   * 
   * @tparam T The type of the elements in the buffer.
   */
  template <typename T>
  using buffer = std::vector<T>;

  /**
   * @brief A buffer of uint8 integer.
   */
  using uint8_buffer = buffer<uint8_t>;

  /**
   * @brief A buffer of uint16 integers.
   */
  using uint16_buffer = buffer<uint16_t>;

  /**
   * @brief A buffer of uint32 integers.
   */
  using uint32_buffer = buffer<uint32_t>;

  /**
   * @brief A buffer of uint64 integers.
   */
  using uint64_buffer = buffer<uint64_t>;

  /**
   * @brief A buffer of int16 integers.
   */
  using int8_buffer = buffer<int8_t>;

  /**
   * @brief A buffer of int16 integers.
   */
  using int16_buffer = buffer<int16_t>;

  /**
   * @brief A buffer of int32_t.
   */
  using int32_buffer = buffer<int32_t>;

  /**
   * @brief A buffer of int64_t.
   */
  using int64_buffer = buffer<int64_t>;

  /**
   * @brief A buffer of floating-point numbers.
   */
  using float32_buffer = buffer<float>;

  /**
   * @brief A buffer of floating-point numbers.
   */
  using float64_buffer = buffer<double>;

} // namespace mgl