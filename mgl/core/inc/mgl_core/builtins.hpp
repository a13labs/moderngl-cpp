/*
   Copyright 2022 Alexandre Pires (c.alexandre.pires@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#pragma once
#include "config.hpp"
#include "pch.hpp"

#define MGL_CORE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace mgl::core
{

  /*************************************************************
   * Generics
   *************************************************************/

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

  template <typename T>
  using span = tcb::span<T>;

  template <typename T>
  using list = std::vector<T>;

  template <typename T, typename U>
  using pair = std::pair<T, U>;

  template <typename T, typename U>
  using dict = std::map<T, U>;

  using path = std::filesystem::path;
  using path_list = mgl::core::list<path>;
  using input_file = std::ifstream;
  using output_file = std::ofstream;
  using openmode = std::ifstream::openmode;
  extern const path null_path;
} // namespace  mgl::core
