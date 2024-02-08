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

namespace mgl::opengl
{
  class texture
  {
public:
    enum type
    {
      TEXTURE_2D,
      TEXTURE_3D,
      TEXTURE_CUBE,
    };

    struct filter
    {
      int min_filter;
      int mag_filter;
    };

    virtual ~texture() = default;

    virtual texture::type texture_type() = 0;

    virtual int32_t width() const = 0;

    virtual int32_t height() const = 0;

    virtual int32_t components() const = 0;

    virtual void use(int index) = 0;

    virtual void release() = 0;

    virtual int32_t glo() const = 0;

    virtual const context_ref& ctx() const = 0;
  };

  using texture_ref = mgl::ref<mgl::opengl::texture>;
  using textures = mgl::ref_list<texture>;

} // namespace  mgl::opengl