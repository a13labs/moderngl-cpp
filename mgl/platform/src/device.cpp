#include "mgl_platform/device.hpp"
#if defined(MGL_RENDER_OPENGL)
#  include "mgl_platform/api/opengl/device.hpp"
#elif defined(MGL_RENDER_METAL)
#  include "mgl_platform/api/metal/api.hpp"
#endif

namespace mgl::platform
{
  device& device::instance()
  {
#if defined(MGL_RENDER_OPENGL)
    static mgl::scope<mgl::platform::api::backends::ogl_api> s_instance =
        mgl::create_scope<mgl::platform::api::backends::ogl_api>();

    return *s_instance;
#elif defined(MGL_RENDER_METAL)
    static mgl::scope<mgl::platform::api::backends::metal_api> s_instance =
        mgl::create_scope<mgl::platform::api::backends::metal_api>();

    return *s_instance;
#endif
  }
}; // namespace mgl::platform::api