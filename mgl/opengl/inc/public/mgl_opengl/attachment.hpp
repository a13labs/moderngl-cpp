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

  class attachment
  {
public:
    enum type
    {
      TEXTURE,
      RENDERBUFFER
    };

    virtual ~attachment() = default;
    virtual attachment::type attachment_type() = 0;

    virtual int32_t width() const = 0;

    virtual int32_t height() const = 0;

    virtual int32_t components() const = 0;

    virtual int32_t samples() const = 0;

    virtual bool depth() const = 0;

    virtual int32_t glo() const = 0;

    virtual const context_ref& ctx() const = 0;
  };

  using attachment_ref = mgl::ref<attachment>;
  using attachments_ref = mgl::ref_list<attachment>;

} // namespace  mgl::opengl