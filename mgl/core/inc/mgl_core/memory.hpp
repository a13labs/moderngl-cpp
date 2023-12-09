#pragma once

#include <memory>
#include <queue>
#include <vector>

namespace mgl
{
  template <typename T>
  using scope = std::unique_ptr<T>;

  template <typename T, typename... Args>
  constexpr scope<T> create_scope(Args&&... args)
  {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }

  template <typename T>
  using ref = std::shared_ptr<T>;

  template <typename T>
  using ref_list = std::vector<ref<T>>;

  template <typename T>
  using ref_queue = std::queue<ref<T>>;

  template <typename T, typename... Args>
  constexpr ref<T> create_ref(Args&&... args)
  {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }

  template <typename T>
  using ref_from_this = std::enable_shared_from_this<T>;

  template <typename T>
  using mem_buffer = std::vector<T>;
} // namespace mgl