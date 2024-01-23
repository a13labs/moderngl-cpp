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

#include "weak_ptr.hpp"
#include <atomic>
#include <mutex>

namespace mgl
{
  template <typename T>
  class weak_ptr;

  template <typename T>
  class shared_ptr;

  template <typename T>
  class shared_ptr
  {
    friend class weak_ptr<T>;

    template <typename U>
    friend class shared_ptr;

private:
    T* m_ptr;
    std::atomic<size_t>* m_weak_ptr_count;

    template <typename U>
    shared_ptr(U* ptr, std::atomic<size_t>* weak_ptr_count)
        : m_ptr(ptr)
        , m_weak_ptr_count(weak_ptr_count)
    {
      if(m_weak_ptr_count)
        m_weak_ptr_count->fetch_add(1, std::memory_order_relaxed);
    }

public:
    shared_ptr()
        : m_ptr(nullptr)
        , m_weak_ptr_count(nullptr)
    { }

    shared_ptr(T* ptr)
        : m_ptr(ptr)
        , m_weak_ptr_count(new std::atomic<size_t>(1))
    { }

    shared_ptr(const shared_ptr<T>& ptr)
        : m_ptr(ptr.m_ptr)
        , m_weak_ptr_count(ptr.m_weak_ptr_count)
    {
      if(m_weak_ptr_count)
        m_weak_ptr_count->fetch_add(1, std::memory_order_relaxed);
    }

    ~shared_ptr()
    {
      if(m_weak_ptr_count)
      {
        m_weak_ptr_count->fetch_sub(1, std::memory_order_relaxed);
        if(*m_weak_ptr_count == 0)
        {
          delete m_ptr;
          delete m_weak_ptr_count;
        }
      }
    }

    template <typename U>
    operator shared_ptr<U>()
    {
      return shared_ptr<U>(dynamic_cast<U*>(m_ptr), m_weak_ptr_count);
    }

    T* get() { return m_ptr; }

    const T* get() const { return m_ptr; }

    T* operator->() const { return m_ptr; }

    T& operator*() { return *m_ptr; }

    shared_ptr<T>& operator=(const shared_ptr<T>& ptr)
    {
      if(this != &ptr)
      {
        if(m_weak_ptr_count && m_weak_ptr_count->fetch_sub(1, std::memory_order_relaxed) == 0)
        {
          delete m_ptr;
          delete m_weak_ptr_count;
        }

        m_ptr = ptr.m_ptr;
        m_weak_ptr_count = ptr.m_weak_ptr_count;
        if(m_weak_ptr_count)
          m_weak_ptr_count->fetch_add(1, std::memory_order_relaxed);
      }
      return *this;
    }

    bool operator==(const shared_ptr<T>& ptr) const { return m_ptr == ptr.m_ptr; }

    bool operator!=(const shared_ptr<T>& ptr) const { return m_ptr != ptr.m_ptr; }

    bool operator==(const T* ptr) const { return m_ptr == ptr; }

    bool operator!=(const T* ptr) const { return m_ptr != ptr; }

    bool operator==(const T& ptr) const { return *m_ptr == ptr; }

    bool operator!=(const T& ptr) const { return *m_ptr != ptr; }

    bool operator!() const { return m_ptr == nullptr; }

    operator bool() const { return m_ptr != nullptr; }

    operator weak_ptr<T>() { return weak_ptr<T>(m_ptr); }
  };

  template <typename T>
  shared_ptr<T> make_shared(T* ptr)
  {
    return shared_ptr<T>(ptr);
  }

  template <typename T, typename... Args>
  shared_ptr<T> make_shared(Args&&... args)
  {
    return shared_ptr<T>(new T(std::forward<Args>(args)...));
  }

} // namespace mgl