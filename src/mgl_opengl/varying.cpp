
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
#include "varying.hpp"
#include "context.hpp"
#include "mgl_core/log.hpp"

namespace mgl::opengl
{
  varying::varying(const mgl::core::string& name, int number, size_t array_length, int dimension)
  {
    m_name = name;
    m_number = number;
    m_array_length = array_length;
    m_dimension = dimension;
  }

} // namespace  mgl::opengl
