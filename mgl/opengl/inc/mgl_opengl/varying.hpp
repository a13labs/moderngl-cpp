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
  class varying
  {
public:
    ~varying() = default;

private:
    friend class context;
    varying(const mgl::core::string& name, int number, size_t array_length, int m_dimension);

    mgl::core::string m_name;
    int m_number;
    size_t m_array_length;
    int m_dimension;
  };

  using varying_ref = mgl::core::ref<varying>;
  using varyings_dict = mgl::core::dict<mgl::core::string, varying_ref>;
} // namespace  mgl::opengl
