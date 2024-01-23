/**
 * @file memory.hpp
 * @brief This file contains the definition of various memory-related utilities.
 */

#pragma once

#include "shared_ptr.hpp"
#include "unique_ptr.hpp"
#include "weak_ptr.hpp"

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
  using scope = mgl::unique_ptr<T>;

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
    return mgl::make_unique<T>(std::forward<Args>(args)...);
  }

  /**
   * @brief A template alias for a shared pointer.
   * 
   * @tparam T The type of the object to be managed.
   */
  template <typename T>
  using ref = mgl::shared_ptr<T>;

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
    return mgl::make_shared<T>(std::forward<Args>(args)...);
  }

  template <typename T>
  using weak_ref = mgl::weak_ptr<T>;

  /**
   * @brief Creates a weak pointer to an object of type T.
   * 
   * @tparam T The type of the object to be created.
   * @tparam Args The types of the arguments to be passed to the constructor of T.
   * @param args The arguments to be passed to the constructor of T.
   * @return A weak pointer to the created object.
   */
  template <typename T, typename... Args>
  constexpr weak_ref<T> create_weak_ref(Args&&... args)
  {
    return mgl::make_weak_ptr<T>(std::forward<Args>(args)...);
  }

  /**
   * @brief A template alias for a weak pointer.
   * 
   * @tparam T The type of the object to be managed.
   * 
   */
  template <typename T, typename U>
  inline shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& ptr) noexcept
  {
    using S = shared_ptr<T>;
    if(auto* cast = dynamic_cast<T*>(ptr.get()))
      return S(ptr, cast);
    return S();
  }

  /**
   * @brief A template alias for a weak pointer.
   * 
   * @tparam T The type of the object to be managed.
   * 
   */
  template <typename T, typename U>
  inline shared_ptr<T> static_pointer_cast(const shared_ptr<U>& ptr) noexcept
  {
    using S = shared_ptr<T>;
    if(auto* cast = static_cast<T*>(ptr.get()))
      return S(ptr, cast);
    return S();
  }

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