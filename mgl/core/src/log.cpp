#include "mgl_core/log.hpp"

extern "C"
{
#include <simplelog/log.h>
}

namespace mgl
{
  namespace log
  {
    void log(level lvl, const std::string& msg, const char* file, int line)
    {
      log_log((int)lvl, file, line, msg.c_str());
    }

  } // namespace log
} // namespace  mgl
