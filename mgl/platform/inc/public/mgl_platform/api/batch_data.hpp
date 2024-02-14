#pragma once

#include "glm/glm.hpp"

namespace mgl::platform::api
{
  struct batch_data
  {
    batch_data()
        : model_view(1.0f)
        , count(0)
        , offset(0)
        , instance_count(1)
    { }

    batch_data(glm::mat4 t, size_t count, size_t offset, size_t instance_count)
        : model_view(t)
        , count(count)
        , offset(offset)
        , instance_count(instance_count)
    { }

    batch_data(const batch_data& other)
        : model_view(other.model_view)
        , count(other.count)
        , offset(other.offset)
        , instance_count(other.instance_count)
    { }

    glm::mat4 model_view;
    size_t count;
    size_t offset;
    size_t instance_count;
  };

} // namespace mgl::platform::api