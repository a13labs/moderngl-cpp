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
#include "mgl_core/math.hpp"

namespace mgl::core
{
  const rect null_viewport_2d = { 0, 0, 0, 0 };
  const cube null_viewport_3d = { 0, 0, 0, 0, 0, 0 };
  const size null_size = { 0, 0 };
  const path null_path = path();
} // namespace  mgl::core