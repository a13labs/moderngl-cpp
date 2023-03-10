#include "registry.hpp"
#include "stb/stb_image.hpp"

namespace mgl::window
{
  namespace resources
  {
    struct settings
    {
      mgl::core::path_list textures_dirs;
      mgl::core::path_list programs_dirs;
    };

    static settings s_settings;

    mipmap_levels null_mipmap_levels = { 0, 0 };
    texture_2d_load_opts texture_2d_load_defaults = { false, true, null_mipmap_levels, 1.0 };
    program_load_opts program_load_defaults = { {}, {} };
    data_load_opts data_load_defaults = { mgl::core::input_file::in | mgl::core::input_file::binary };

    bool append_unique_path(const mgl::core::string& value, mgl::core::path_list& list)
    {

      auto path = mgl::core::path(value);

      if(path.is_relative())
      {
        MGL_CORE_ERROR("resources: path is relative: {0}", value);
        return false;
      }

      if(!std::filesystem::exists(path))
      {
        MGL_CORE_ERROR("resources: path does not exist: {0}", value);
        return false;
      }

      if(!std::filesystem::is_directory(path))
      {
        MGL_CORE_ERROR("resources: path is not a directory: {0}", value);
        return false;
      }

      auto it = std::find(list.begin(), list.end(), path);

      if(it == list.end())
        return true;

      list.push_back(path);
      return true;
    }

    const mgl::core::path& find(const mgl::core::string& value, mgl::core::path_list& list)
    {
      for(auto&& base : list)
      {
        auto path = std::filesystem::path() / value;

        if(std::filesystem::exists(path))
        {
          return base;
        }
      }

      return mgl::core::null_path;
    }

    bool register_dir(const mgl::core::string& path)
    {
      auto result = register_program_dir(path);
      result &= register_texture_dir(path);
      result &= register_scene_dir(path);
      result &= register_data_dir(path);
      return result;
    }

    bool register_program_dir(const mgl::core::string& path)
    {
      return append_unique_path(path, s_settings.programs_dirs);
    }

    bool register_texture_dir(const mgl::core::string& path)
    {
      return append_unique_path(path, s_settings.textures_dirs);
    }

    bool register_scene_dir(const mgl::core::string& path)
    {
      return false;
    }

    bool register_data_dir(const mgl::core::string& path)
    {
      return false;
    }

    static void horizontal_flip(void* image, int w, int h, int components)
    {
      size_t line_bytes = (size_t)w * components;
      stbi_uc temp[line_bytes];
      stbi_uc* bytes = (stbi_uc*)image;
      int lpos, rpos;
      for(int col = 0; col < h; col++)
      {
        stbi_uc* line = bytes + col * line_bytes;
        memcpy(&temp, line, line_bytes);
        for(int row = 0; row < w; row++)
        {
          lpos = row * components;
          rpos = line_bytes - row * components - 1;
          line[lpos] = temp[rpos - 3];
          line[lpos + 1] = temp[rpos - 2];
          line[lpos + 2] = temp[rpos - 1];
          line[lpos + 3] = temp[rpos];
        }
      }
    }

    mgl::core::ref<mgl::opengl::texture> load_texture_2d(const mgl::core::string& path, const texture_2d_load_opts& opts)
    {
      MGL_CORE_ASSERT(window::current().context(), "No context!");
      const auto ctx = window::current().context();

      auto base_path = find(path, s_settings.textures_dirs);

      if(base_path.empty())
      {
        MGL_CORE_ERROR("load_texture_2d: path not found: {0}", path);
        return nullptr;
      }

      auto full_path = base_path / path;

      int width, height, components;
      stbi_set_flip_vertically_on_load(opts.flip_y);
      unsigned char* data = stbi_load(full_path.c_str(), &width, &height, &components, 0);

      if(opts.flip_x)
      {
        horizontal_flip(data, width, height, components);
      }

      auto texture = ctx->texture2d(width, height, components, data);
      stbi_image_free(data);

      if(opts.mipmap_levels != null_mipmap_levels)
      {
        texture->build_mipmaps(opts.mipmap_levels.base, opts.mipmap_levels.max_level);
      }

      texture->set_anisotropy(opts.anisotropy);

      return texture;
    }

    bool load_data_file(const mgl::core::string& path, mgl::core::input_file& file, const data_load_opts& opts)
    {
      auto base_path = find(path, s_settings.textures_dirs);

      if(base_path.empty())
      {
        MGL_CORE_ERROR("load_data_file: path not found: {0}", path);
        return false;
      }

      auto full_path = base_path / path;

      file.open(full_path, opts.mode);
      return true;
    }

    mgl::core::ref<mgl::opengl::program> load_program(const mgl::core::string& path, const program_load_opts& opts)
    {
      MGL_CORE_ASSERT(window::current().context(), "No context!");
      const auto ctx = window::current().context();

      auto base_path = find(path, s_settings.textures_dirs);

      if(base_path.empty())
      {
        MGL_CORE_ERROR("load_program: path not found: {0}", path);
        return nullptr;
      }

      auto full_path = base_path / path;

      mgl::core::input_file shader_file(full_path, mgl::core::input_file::in);
      mgl::core::string shader_text((std::istreambuf_iterator<char>(shader_file)), std::istreambuf_iterator<char>());

      mgl::opengl::glsl_source shader_source(shader_text);
      mgl::opengl::glsl_sources glsl = { shader_source };
      mgl::opengl::shaders_outputs outputs = opts.outputs;

      if(outputs.size() == 0 && glsl.fragment().empty())
      {
        MGL_CORE_ASSERT(!glsl.vertex().empty() || !glsl.geometry().empty(), "Required vertex or geometry shader");
        outputs = shader_source.outputs();
      }

      return ctx->program(glsl, outputs);
    }

    mgl::core::ref<mgl::opengl::program> load_program(mgl::opengl::glsl_source& source, const program_load_opts& opts)
    {
      MGL_CORE_ASSERT(window::current().context(), "No context!");
      const auto ctx = window::current().context();

      mgl::opengl::glsl_sources glsl = { source };

      MGL_CORE_ASSERT(!glsl.empty(), "Empty source");
      mgl::opengl::shaders_outputs outputs = opts.outputs;

      return ctx->program(glsl, outputs);
    }

  } // namespace resources
} // namespace  mgl::window