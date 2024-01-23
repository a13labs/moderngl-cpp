/*
 * Copyright (c), A13Labs (c.alexandre.pires@gmail.com).
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

/*!
 * @file smartpointer.hpp
 * @brief Smart pointer header file, implements a generic smart pointer including shared and weak pointers and a weak_ptrerence counter, also a unique pointer is implemented
 * @author Alexandre Pires (c.alexandre.pires@gmail.com)
 * @date 28 Jul 2023
 */
#pragma once

#include "utils.hpp"
#include <atomic>
#include <mutex>

namespace mgl
{
  template <typename T>
  class shared_ptr;

  template <typename T>
  class weak_ptr
  {
private:
    T* m_ptr;

public:
    weak_ptr()
        : m_ptr(nullptr)
    { }

    weak_ptr(T* ptr)
        : m_ptr(ptr)
    { }

    weak_ptr(const weak_ptr<T>& ptr)
        : m_ptr(ptr.m_ptr)
    { }

    weak_ptr(const shared_ptr<T>& ptr)
        : m_ptr(ptr.m_ptr)
    { }

    ~weak_ptr() { }

    T* get() { return m_ptr; }

    T* operator->() const { return m_ptr; }

    T& operator*() { return *m_ptr; }

    template <typename U>
    operator weak_ptr<U>()
    {
      return weak_ptr<U>(static_cast<U*>(m_ptr));
    }

    weak_ptr<T>& operator=(const weak_ptr<T>& ptr)
    {
      if(this != &ptr)
      {
        m_ptr = ptr.m_ptr;
      }
      return *this;
    }

    bool operator==(const weak_ptr<T>& ptr) const { return m_ptr == ptr.m_ptr; }

    bool operator!=(const weak_ptr<T>& ptr) const { return m_ptr != ptr.m_ptr; }

    bool operator==(const T* ptr) const { return m_ptr == ptr; }

    bool operator!=(const T* ptr) const { return m_ptr != ptr; }

    bool operator==(const T& ptr) const { return *m_ptr == ptr; }

    bool operator!=(const T& ptr) const { return *m_ptr != ptr; }

    bool operator!() const { return m_ptr == nullptr; }

    operator bool() const { return m_ptr != nullptr; }
  };

  template <typename T>
  weak_ptr<T> make_weak_ptr(T* ptr)
  {
    return weak_ptr<T>(ptr);
  }

  template <typename T, typename... Args>
  weak_ptr<T> make_weak_ptr(Args&&... args)
  {
    return weak_ptr<T>(new T(std::forward<Args>(args)...));
  }

} // namespace mgl