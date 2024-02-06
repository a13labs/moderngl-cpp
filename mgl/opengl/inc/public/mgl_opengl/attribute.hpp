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

#include "data_type.hpp"

#include "mgl_core/string.hpp"

namespace mgl::opengl
{
  class attribute
  {
public:
    struct data_type
    {
      int32_t dimension;
      int32_t scalar_type;
      int32_t rows_length;
      int32_t row_length;
      bool normalizable;
    };

    attribute(const std::string& name,
              int32_t gl_type,
              int32_t program_obj,
              int32_t location,
              size_t array_length);

    ~attribute() = default;

    const std::string& name() const { return m_name; }

    int32_t gl_type() const { return m_gl_type; }

    int32_t location() const { return m_location; }

    size_t array_length() const { return m_array_length; }

    const data_type* get_data_type() const { return m_data_type; }

private:
    std::string m_name;
    int32_t m_program_obj;
    int32_t m_gl_type;
    data_type* m_data_type;
    int32_t m_location;
    size_t m_array_length;
  };

  using attribute_ref = mgl::ref<attribute>;
  using attributes_dict = mgl::dict<std::string, attribute_ref>;

} // namespace  mgl::opengl
