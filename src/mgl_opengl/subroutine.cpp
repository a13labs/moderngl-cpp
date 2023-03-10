
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
#include "subroutine.hpp"
#include "context.hpp"
#include "mgl_core/log.hpp"

namespace mgl::opengl
{
  subroutine::subroutine(const mgl::core::string& name, int index, subroutine::type type)
  {
    m_name = name;
    m_index = index;
    m_type = type;
  }

} // namespace  mgl::opengl
