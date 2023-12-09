#pragma once

#include "containers.hpp"

#define BIT(x) 1 << x

namespace mgl
{

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