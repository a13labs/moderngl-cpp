/**
 * @file log.hpp
 * @brief This file contains the declaration of the logging functionality for the mgl::log namespace.
 *
 * The mgl::log namespace provides functions for logging messages at different levels of severity.
 * It also includes macros for convenient logging.
 */

#pragma once
#include "memory.hpp"
#include "string.hpp"

#include <format>

namespace mgl::log
{
  /**
   * @brief Enumeration of log levels.
   */
  enum class level
  {
    trace, /**< Trace level */
    debug, /**< Debug level */
    info, /**< Info level */
    warn, /**< Warning level */
    error, /**< Error level */
    critical /**< Critical level */
  };

  /**
   * @brief Logs a message at the specified log level.
   *
   * @param lvl The log level.
   * @param msg The message to log.
   */
  void log(level lvl, const std::string& msg);

  /**
   * @brief Logs a trace message with formatted arguments.
   *
   * @tparam Args The types of the arguments.
   * @param fmt The format string.
   * @param args The arguments to format.
   */
  template <typename... Args>
  void trace(std::format_string<Args...> fmt, Args&&... args)
  {
    std::string msg = std::format(fmt, std::forward<Args>(args)...);
    log(level::trace, msg);
  }

  /**
   * @brief Logs a debug message with formatted arguments.
   *
   * @tparam Args The types of the arguments.
   * @param fmt The format string.
   * @param args The arguments to format.
   */
  template <typename... Args>
  void debug(std::format_string<Args...> fmt, Args&&... args)
  {
    std::string msg = std::format(fmt, std::forward<Args>(args)...);
    log(level::debug, msg);
  }

  /**
   * @brief Logs an info message with formatted arguments.
   *
   * @tparam Args The types of the arguments.
   * @param fmt The format string.
   * @param args The arguments to format.
   */
  template <typename... Args>
  void info(std::format_string<Args...> fmt, Args&&... args)
  {
    std::string msg = std::format(fmt, std::forward<Args>(args)...);
    log(level::info, msg);
  }

  /**
   * @brief Logs a warning message with formatted arguments.
   *
   * @tparam Args The types of the arguments.
   * @param fmt The format string.
   * @param args The arguments to format.
   */
  template <typename... Args>
  void warn(std::format_string<Args...> fmt, Args&&... args)
  {
    std::string msg = std::format(fmt, std::forward<Args>(args)...);
    log(level::warn, msg);
  }

  /**
   * @brief Logs an error message with formatted arguments.
   *
   * @tparam Args The types of the arguments.
   * @param fmt The format string.
   * @param args The arguments to format.
   */
  template <typename... Args>
  void error(std::format_string<Args...> fmt, Args&&... args)
  {
    std::string msg = std::format(fmt, std::forward<Args>(args)...);
    log(level::error, msg);
  }

  /**
   * @brief Logs a critical message with formatted arguments.
   *
   * @tparam Args The types of the arguments.
   * @param fmt The format string.
   * @param args The arguments to format.
   */
  template <typename... Args>
  void critical(std::format_string<Args...> fmt, Args&&... args)
  {
    std::string msg = std::format(fmt, std::forward<Args>(args)...);
    log(level::critical, msg);
  }

} // namespace mgl::log

// log macros
#define MGL_CORE_TRACE(...) ::mgl::log::trace(__VA_ARGS__)
#define MGL_CORE_INFO(...) ::mgl::log::info(__VA_ARGS__)
#define MGL_CORE_WARN(...) ::mgl::log::warn(__VA_ARGS__)
#define MGL_CORE_ERROR(...) ::mgl::log::error(__VA_ARGS__)
#define MGL_CORE_CRITICAL(...) ::mgl::log::critical(__VA_ARGS__)
