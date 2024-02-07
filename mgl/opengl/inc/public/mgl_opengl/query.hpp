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