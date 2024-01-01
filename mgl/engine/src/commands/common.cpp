#include "mgl_engine/commands/common.hpp"

#include "mgl_core/log.hpp"

#include <glm/gtx/string_cast.hpp>

namespace mgl::engine
{
  void clear_command::execute()
  {
    if(renderer() == nullptr)
      return;

    auto ctx = renderer()->context();

    ctx->clear(m_color);
  }

  void set_viewport_command::execute()
  {
    if(renderer() == nullptr)
      return;

    auto ctx = renderer()->context();

    MGL_CORE_INFO("Setting viewport: {} {}", glm::to_string(m_position), glm::to_string(m_size));
  }

  void set_view_command::execute()
  {
    if(renderer() == nullptr)
      return;

    auto ctx = renderer()->context();

    MGL_CORE_INFO("Setting view: {}", glm::to_string(m_view));
  }

  void set_projection_command::execute()
  {
    if(renderer() == nullptr)
      return;

    auto ctx = renderer()->context();

    MGL_CORE_INFO("Setting projection: {}", glm::to_string(m_projection));
  }
} // namespace mgl::engine