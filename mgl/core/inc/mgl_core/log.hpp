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
#include "memory.hpp"
#include "string.hpp"

#include <format>

namespace mgl::log
{
  enum class level
  {
    trace,
    debug,
    info,
    warn,
    error,
    critical
  };

  void init(const std::string& file = "app.log");

  void log(level lvl, const std::string& msg);

  template <typename... Args>
  void trace(std::format_string<Args...> fmt, Args&&... args)
  {
    std::string msg = std::format(fmt, std::forward<Args>(args)...);
    log(level::trace, msg);
  }

  template <typename... Args>
  void debug(std::format_string<Args...> fmt, Args&&... args)
  {
    std::string msg = std::format(fmt, std::forward<Args>(args)...);
    log(level::debug, msg);
  }

  template <typename... Args>
  void info(std::format_string<Args...> fmt, Args&&... args)
  {
    std::string msg = std::format(fmt, std::forward<Args>(args)...);
    log(level::info, msg);
  }

  template <typename... Args>
  void warn(std::format_string<Args...> fmt, Args&&... args)
  {
    std::string msg = std::format(fmt, std::forward<Args>(args)...);
    log(level::warn, msg);
  }

  template <typename... Args>
  void error(std::format_string<Args...> fmt, Args&&... args)
  {
    std::string msg = std::format(fmt, std::forward<Args>(args)...);
    log(level::error, msg);
  }

  template <typename... Args>
  void critical(std::format_string<Args...> fmt, Args&&... args)
  {
    std::string msg = std::format(fmt, std::forward<Args>(args)...);
    log(level::critical, msg);
  }

  bool is_initialized();

} // namespace mgl::log

// log macros
#define MGL_CORE_TRACE(...) ::mgl::log::trace(__VA_ARGS__)
#define MGL_CORE_INFO(...) ::mgl::log::info(__VA_ARGS__)
#define MGL_CORE_WARN(...) ::mgl::log::warn(__VA_ARGS__)
#define MGL_CORE_ERROR(...) ::mgl::log::error(__VA_ARGS__)
#define MGL_CORE_CRITICAL(...) ::mgl::log::critical(__VA_ARGS__)
