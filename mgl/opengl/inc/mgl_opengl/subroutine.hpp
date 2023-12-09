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

#include "mgl_core/containers.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"

namespace mgl::opengl
{
  class context;

  class subroutine
  {
    enum type
    {
      VERTEX_SHADER = 0x8B31,
      FRAGMENT_SHADER = 0x8B30,
      GEOMETRY_SHADER = 0x8DD9,
      TESS_EVALUATION_SHADER = 0x8E87,
      TESS_CONTROL_SHADER = 0x8E88,
    };

public:
    ~subroutine() = default;

private:
    friend class context;
    subroutine(const std::string& name, int index, subroutine::type type);

    std::string m_name;
    int m_index;
    subroutine::type m_type;
  };

  using subroutine_ref = mgl::ref<subroutine>;
  using subroutines_dict = mgl::dict<std::string, subroutine_ref>;

} // namespace  mgl::opengl