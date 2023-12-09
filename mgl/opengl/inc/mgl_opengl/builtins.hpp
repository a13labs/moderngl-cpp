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

#include "mgl_core/string.hpp"
#include "mgl_core/utils.hpp"

namespace mgl::opengl
{
  using gl_function = void*;
  using gl_context = void*;

  enum render_mode
  {
    POINTS = 0x0000,
    LINES = 0x0001,
    LINE_LOOP = 0x0002,
    LINE_STRIP = 0x0003,
    TRIANGLES = 0x0004,
    TRIANGLE_STRIP = 0x0005,
    TRIANGLE_FAN = 0x0006,
    LINES_ADJACENCY = 0x000A,
    LINE_STRIP_ADJACENCY = 0x000B,
    TRIANGLES_ADJACENCY = 0x000C,
    TRIANGLE_STRIP_ADJACENCY = 0x000D,
    PATCHES = 0x000E,
  };

  enum enable_flag
  {
    NOTHING = 0,
    BLEND = BIT(1),
    DEPTH_TEST = BIT(2),
    CULL_FACE = BIT(3),
    RASTERIZER_DISCARD = BIT(4),
    PROGRAM_POINT_SIZE = BIT(5),
    INVALID = 0x40000000
  };

  enum compare_func
  {
    NONE = 0x0000,
    NEVER = 0x0200,
    LESS = 0x0201,
    EQUAL = 0x0202,
    LESS_EQUAL = 0x0203,
    GREATER = 0x0204,
    NOT_EQUAL = 0x0205,
    GREATER_EQUAL = 0x0206,
    ALWAYS = 0x0207,
  };

  struct color_mask
  {
    bool r, g, b, a;
  };

  struct data_type;

  struct glsl_sources;
  class glsl_source;

  class attribute;
  using attribute_ref = mgl::core::ref<mgl::opengl::attribute>;

  class attachment;
  using attachment_ref = mgl::core::ref<mgl::opengl::attachment>;

  class buffer;
  using buffer_ref = mgl::core::ref<mgl::opengl::buffer>;

  class context;
  using context_ref = mgl::core::ref<mgl::opengl::context>;

  class conditional_render;
  using conditional_render_ref = mgl::core::ref<mgl::opengl::conditional_render>;

  class compute_shader;
  using compute_shader_ref = mgl::core::ref<mgl::opengl::compute_shader>;

  class framebuffer;
  using framebuffer_ref = mgl::core::ref<mgl::opengl::framebuffer>;

  class program;
  using program_ref = mgl::core::ref<mgl::opengl::program>;

  class query;
  using query_ref = mgl::core::ref<mgl::opengl::query>;

  class renderbuffer;
  using renderbuffer_ref = mgl::core::ref<mgl::opengl::renderbuffer>;

  class sampler;
  using sampler_ref = mgl::core::ref<mgl::opengl::sampler>;

  class scope;
  using scope_ref = mgl::core::ref<mgl::opengl::scope>;

  class subroutine;
  using subroutine_ref = mgl::core::ref<mgl::opengl::subroutine>;

  class texture;
  using texture_ref = mgl::core::ref<mgl::opengl::texture>;

  class texture_2d;
  using texture_2d_ref = mgl::core::ref<mgl::opengl::texture_2d>;

  class texture_array;
  using texture_array_ref = mgl::core::ref<mgl::opengl::texture_array>;

  class texture_3d;
  using texture_3d_ref = mgl::core::ref<mgl::opengl::texture_3d>;

  class texture_cube;
  using texture_cube_ref = mgl::core::ref<mgl::opengl::texture_cube>;

  class uniform;
  using uniform_ref = mgl::core::ref<mgl::opengl::uniform>;

  class uniform_block;
  using uniform_block_ref = mgl::core::ref<mgl::opengl::uniform_block>;

  class varying;
  using varying_ref = mgl::core::ref<mgl::opengl::varying>;

  class vertex_array;
  using vertex_array_ref = mgl::core::ref<mgl::opengl::vertex_array>;

  struct texture_binding
  {
    texture_ref texture;
    int binding;
  };

  struct buffer_binding
  {
    buffer_ref buffer;
    int binding;
  };

  struct sampler_binding
  {
    sampler_ref sampler;
    int binding;
  };

  using shader_defines = mgl::core::dict<mgl::core::string, mgl::core::string>;
  using attributes_dict = mgl::core::dict<mgl::core::string, attribute_ref>;
  using subroutines_dict = mgl::core::dict<mgl::core::string, subroutine_ref>;
  using uniforms_dict = mgl::core::dict<mgl::core::string, uniform_ref>;
  using uniform_blocks_dict = mgl::core::dict<mgl::core::string, uniform_block_ref>;
  using varyings_dict = mgl::core::dict<mgl::core::string, varying_ref>;
  using attachments_ref = mgl::core::ref_list<attachment>;
  using shaders_outputs = mgl::core::string_list;
  using fragment_outputs = mgl::core::dict<mgl::core::string, int>;
  using color_masks = mgl::core::list<color_mask>;
  using textures = mgl::core::ref_list<texture>;
  using buffers = mgl::core::ref_list<buffer>;
  using samplers = mgl::core::ref_list<sampler>;
  using texture_bindings = mgl::core::list<texture_binding>;
  using buffer_bindings = mgl::core::list<buffer_binding>;
  using sampler_bindings = mgl::core::list<sampler_binding>;

  extern const std::string no_shader;

  struct vertex_buffer
  {
    buffer_ref buffer;
    mgl::core::string buffer_layout;
    mgl::core::string_list attributes;
  };

  using vertex_buffer_list = mgl::core::list<vertex_buffer>;
} // namespace  mgl::opengl
