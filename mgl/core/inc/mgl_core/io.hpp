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

#include "containers.hpp"
#include "memory.hpp"

#include <filesystem>
#include <fstream>

namespace mgl
{
  using path = std::filesystem::path;
  using path_list = mgl::list<path>;
  using input_file = std::ifstream;
  using output_file = std::ofstream;
  using openmode = std::ifstream::openmode;
  using ifsteam = std::istream;
  using ofsteam = std::ostream;

  using ifsteam_ptr = mgl::ref<ifsteam>;
  using ofsteam_ptr = mgl::ref<ofsteam>;

  extern const path null_path;

} // namespace  mgl
