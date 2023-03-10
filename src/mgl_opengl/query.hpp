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
#include "builtins.hpp"

namespace mgl::opengl
{
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

    int samples();
    int primitives();
    int elapsed();

private:
    friend class context;
    query() = default;

    context* m_context;
    int m_query_obj[query::keys::COUNT];
  };
} // namespace  mgl::opengl