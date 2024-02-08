#pragma once
#include "enums.hpp"
#include "gl_object.hpp"

#include "mgl_core/memory.hpp"

#include "glm/vec4.hpp"

namespace mgl::opengl
{

  class sampler : public gl_object
  {
public:
    struct filter
    {
      int32_t min_filter;
      int32_t mag_filter;
    };

    ~sampler() = default;

public:
    void use(int32_t index = 0);
    void clear(int32_t index = 0);

    virtual void release() override final;

    void set_repeat_x(bool value);

    bool repeat_x() const { return m_repeat_x; }

    void set_repeat_y(bool value);

    bool repeat_y() const { return m_repeat_y; }

    void set_repeat_z(bool value);

    bool repeat_z() const { return m_repeat_z; }

    void set_anisotropy(float value);

    float anisotropy() const { return m_anisotropy; }

    float min_lod() const { return m_min_lod; }

    void set_min_lod(float value);

    float max_lod() const { return m_max_lod; }

    void set_max_lod(float value);

    const glm::vec4& border_color() const { return m_border_color; }

    void set_border_color(const glm::vec4& value);

    void set_border_color(float r, float g, float b, float a) { set_border_color({ r, g, b, a }); }

    void set_filter(const filter& value);

    const filter& sampler_filter() const { return m_filter; }

    void set_compare_func(mgl::opengl::compare_func value);

    mgl::opengl::compare_func compare_func() { return m_compare_func; }

private:
    friend class context;
    sampler(const context_ref& ctx);

    float m_anisotropy;
    bool m_repeat_x;
    bool m_repeat_y;
    bool m_repeat_z;
    float m_min_lod;
    float m_max_lod;
    filter m_filter;
    glm::vec4 m_border_color;
    mgl::opengl::compare_func m_compare_func;
  };

  using sampler_ref = mgl::ref<sampler>;
  using samplers = mgl::ref_list<sampler>;

} // namespace  mgl::opengl
