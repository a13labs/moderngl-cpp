#pragma once

#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"
#include "mgl_registry/resource.hpp"

namespace mgl::registry
{

  class text;
  using text_ref = mgl::ref<text>;

  class text : public resource
  {
public:
    text(const std::string& data)
        : m_data(data)
    { }
    ~text() = default;

    text(const text&) = delete;
    text(text&&) = delete;
    text& operator=(const text&) = delete;
    text& operator=(text&&) = delete;

    virtual resource::type get_type() const override { return resource::type::text; }

    const std::string data() const { return m_data; }

private:
    std::string m_data;
  };

} // namespace mgl::registry