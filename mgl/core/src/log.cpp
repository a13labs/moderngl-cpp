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
#include "mgl_core/log.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace mgl
{
  namespace log
  {
    static ref<spdlog::logger> s_logger = nullptr;

    void init(const std::string& logfile)
    {

      std::vector<spdlog::sink_ptr> logSinks;

      // By default we have only a file logger
      logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(logfile, true));
      logSinks[0]->set_pattern("[%T] [%l] %n: %v");

#ifdef MGL_DEBUG
      // For debugging purposes we attach a logger to the console
      logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
      logSinks[1]->set_pattern("%^[%T] %n: %v%$");
#endif

      s_logger = std::make_shared<spdlog::logger>("mgl_core", begin(logSinks), end(logSinks));
      spdlog::register_logger(s_logger);
      s_logger->set_level(spdlog::level::trace);
      s_logger->flush_on(spdlog::level::trace);
    }

    void log(level lvl, const std::string& msg)
    {
      s_logger->log(static_cast<spdlog::level::level_enum>(lvl), msg);
    }

    bool is_initialized()
    {
      return s_logger != nullptr;
    }

  } // namespace log
} // namespace  mgl
