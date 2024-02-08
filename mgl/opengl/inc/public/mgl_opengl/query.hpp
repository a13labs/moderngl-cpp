#pragma once

#include "mgl_core/memory.hpp"
namespace mgl::opengl
{
  class context;
  using context_ref = mgl::ref<context>;

  class query
  {
public:
    enum keys
    {
      SAMPLES_PASSED,
      ANY_SAMPLES_PASSED,
      TIME_ELAPSED,
      PRIMITIVES_GENERATED,

      COUNT,
    };
    ~query() = default;

    void begin();
    void end();
    void begin_render();
    void end_render();

    int32_t samples();
    int32_t primitives();
    int32_t elapsed();

    bool released() const
    {
      return m_glo[0] == 0 && m_glo[1] == 0 && m_glo[2] == 0 && m_glo[3] == 0;
    }

    int32_t glo(int32_t key) const { return m_glo[key]; }

    context_ref& ctx() { return m_ctx; }

private:
    friend class context;
    query(const context_ref& ctx,
          bool samples,
          bool any_samples,
          bool time_elapsed,
          bool primitives_generated);

    context_ref m_ctx;
    int32_t m_glo[query::keys::COUNT];
  };

  using query_ref = mgl::ref<query>;

} // namespace  mgl::opengl