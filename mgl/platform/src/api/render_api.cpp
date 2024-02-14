#include "mgl_platform/api/render_api.hpp"
#include "mgl_platform/api/opengl/api.hpp"

namespace mgl::platform::api
{
  render_api& render_api::instance()
  {
    static mgl::scope<mgl::platform::api::backends::ogl_api> s_instance =
        mgl::create_scope<mgl::platform::api::backends::ogl_api>();

    return *s_instance;
  }
}; // namespace mgl::platform::api