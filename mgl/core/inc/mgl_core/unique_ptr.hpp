/*
 * Copyright (c), A13Labs (c.alexandre.pires@gmail.com).
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

/*!
 * @file smartpointer.hpp
 * @brief Smart pointer header file, implements a generic smart pointer including shared and weak pointers and a reference counter, also a unique pointer is implemented
 * @author Alexandre Pires (c.alexandre.pires@gmail.com)
 * @date 28 Jul 2023
 */
#pragma once
#include <atomic>
#include <mutex>

namespace mgl
{
  template <typename T>
  class unique_ptr;

  template <typename T>
  class unique_ptr
  {
private:
    T* m_ptr;

public:
    unique_ptr()
        : m_ptr(nullptr)
    { }

    unique_ptr(T* ptr)
        : m_ptr(ptr)
    { }

    unique_ptr(const unique_ptr<T>& ptr)
    {
      m_ptr = ptr.m_ptr;
      ((unique_ptr<T>&)ptr).release();
    }

    ~unique_ptr()
    {
      if(m_ptr)
      {
        delete m_ptr;
      }
    }

    T* get() { return m_ptr; }

    const T* get() const { return m_ptr; }

    T* operator->() const { return m_ptr; }

    T& operator*() { return *m_ptr; }

    unique_ptr<T>& operator=(const unique_ptr<T>& ptr)
    {
      if(this != &ptr)
      {
        if(m_ptr)
        {
          delete m_ptr;
        }

        m_ptr = ptr.m_ptr;

        ((unique_ptr<T>&)ptr).release();
      }
      return *this;
    }

    bool operator==(const unique_ptr<T>& ptr) const { return m_ptr == ptr.m_ptr; }

    bool operator!=(const unique_ptr<T>& ptr) const { return m_ptr != ptr.m_ptr; }

    bool operator==(const T* ptr) const { return m_ptr == ptr; }

    bool operator!=(const T* ptr) const { return m_ptr != ptr; }

    bool operator==(const T& ptr) const { return *m_ptr == ptr; }

    bool operator!=(const T& ptr) const { return *m_ptr != ptr; }

    bool operator!() const { return m_ptr == nullptr; }

    operator bool() const { return m_ptr != nullptr; }

    template <typename U>
    operator unique_ptr<U>()
    {
      auto new_ptr = static_cast<U*>(m_ptr);

      if(new_ptr)
      {
        release();
        return unique_ptr<U>(new_ptr);
      }

      return unique_ptr<U>();
    }

    void release() { m_ptr = nullptr; }

    void reset(T* ptr)
    {
      if(m_ptr)
      {
        delete m_ptr;
      }
      m_ptr = ptr;
    }

    void swap(unique_ptr<T>& ptr)
    {
      auto tmp = m_ptr;
      m_ptr = ptr.m_ptr;
      ptr.m_ptr = tmp;
    }
  };

  template <typename T>
  unique_ptr<T> make_unique(T* ptr)
  {
    return unique_ptr<T>(ptr);
  }

  template <typename T, typename... Args>
  unique_ptr<T> make_unique(Args&&... args)
  {
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
  }

} // namespace mgl