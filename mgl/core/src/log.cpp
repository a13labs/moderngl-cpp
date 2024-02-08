#include "mgl_core/log.hpp"

extern "C"
{
#include <simplelog/log.h>
}

namespace mgl
{
  namespace log
  {
    void log(level lvl, const std::string& msg)
    {
      log_log((int)lvl, __FILE__, __LINE__, msg.c_str());
    }

  } // namespace log
} // namespace  mgl
