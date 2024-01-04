#include "mgl_window/resources/registry.hpp"
#include "mgl_window/window.hpp"

#include "stb/stb_image.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/zip.hpp"
#include "mgl_opengl/texture_2d.hpp"

namespace mgl::window
{
  namespace resources
  {
    struct location
    {
      mgl::path path;
      bool is_zip;

      location(const std::string& path, bool is_zip)
          : path(path)
          , is_zip(is_zip)
      { }

      location(const mgl::path& path, bool is_zip)
          : path(path.string())
          , is_zip(is_zip)
      { }

      location(const location& other)
          : path(other.path)
          , is_zip(other.is_zip)
      { }

      bool operator==(const location& other) const
      {
        return path == other.path && is_zip == other.is_zip;
      }

      bool is_null() const { return path == mgl::null_path; }
    };

    location null_location = { mgl::null_path, false };

    struct settings
    {
      mgl::list<location> textures_dirs;
      mgl::list<location> programs_dirs;
    };

    static bool s_phyfs_initialized = false;

    static settings s_settings;

    mipmap_levels null_mipmap_levels = { 0, 0 };
    texture_2d_load_opts texture_2d_load_defaults = { false, true, null_mipmap_levels, 1.0 };
    program_load_opts program_load_defaults = { {}, {} };
    data_load_opts data_load_defaults = { mgl::input_file::in | mgl::input_file::binary };

    bool append_unique_path(const std::string& value, mgl::list<location>& list)
    {
      auto path = mgl::path(value);

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

      bool is_zip = false;

      if(!std::filesystem::is_directory(path))
      {
        if(!mgl::zip_file::is_zip_file(path))
        {
          MGL_CORE_ERROR("resources: path is not a directory neither zip file: {0}", value);
          return false;
        }
        is_zip = true;
      }

      auto it = std::find(list.begin(), list.end(), (location){ path, is_zip });

      if(it == list.end())
        list.push_back({ path, is_zip });

      return true;
    }

    const location& find(const std::string& value, mgl::list<location>& list)
    {
      for(auto&& base : list)
      {

        if(base.is_zip)
        {
          mgl::zip_file zip(base.path);
          if(!zip.exists(value))
          {
            continue;
          }
          return base;
        }

        auto path = base.path / value;

        if(std::filesystem::exists(path))
        {
          return base;
        }
      }

      return null_location;
    }

    bool register_dir(const std::string& path)
    {
      auto result = register_program_dir(path);
      result &= register_texture_dir(path);
      result &= register_scene_dir(path);
      result &= register_data_dir(path);
      return result;
    }

    bool register_program_dir(const std::string& path)
    {
      return append_unique_path(path, s_settings.programs_dirs);
    }

    bool register_texture_dir(const std::string& path)
    {
      return append_unique_path(path, s_settings.textures_dirs);
    }

    bool register_scene_dir(const std::string& path)
    {
      return false;
    }

    bool register_data_dir(const std::string& path)
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
        std::copy(&temp[0], &temp[line_bytes], &line[0]);
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

    mgl::window::api::texture_ref load_texture_2d(const std::string& path,
                                                  const texture_2d_load_opts& opts)
    {
      MGL_CORE_ASSERT(mgl::window::current_context(), "No context!");
      const auto ctx = mgl::window::current_context();

      auto base_location = find(path, s_settings.textures_dirs);

      if(base_location.is_null())
      {
        MGL_CORE_ERROR("load_texture_2d: path not found: {0}", path);
        return nullptr;
      }

      int width, height, components;
      stbi_set_flip_vertically_on_load(opts.flip_y);

      unsigned char* data = nullptr;

      if(base_location.is_zip)
      {
        mgl::zip_file zip(base_location.path);
        mgl::buffer<uint8_t> buffer;
        zip.read(path, buffer);
        data = stbi_load_from_memory(buffer.data(), buffer.size(), &width, &height, &components, 0);
      }
      else
      {
        auto full_path = base_location.path / path;

        data = stbi_load((const char*)full_path.c_str(), &width, &height, &components, 0);
      }

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

    bool load_data_file(const std::string& path, mgl::input_file& file, const data_load_opts& opts)
    {
      auto base_location = find(path, s_settings.textures_dirs);

      if(base_location.is_null())
      {
        MGL_CORE_ERROR("load_data_file: path not found: {0}", path);
        return false;
      }

      if(base_location.is_zip)
      {
        MGL_CORE_ERROR("load_data_file: zip files not supported: {0}", path);
        return false;
      }

      auto full_path = base_location.path / path;

      file.open(full_path, opts.mode);
      return true;
    }

    mgl::window::api::program_ref load_program(const std::string& path,
                                               const program_load_opts& opts)
    {
      MGL_CORE_ASSERT(mgl::window::current_context(), "No context!");
      const auto ctx = mgl::window::current_context();

      auto base_location = find(path, s_settings.textures_dirs);

      if(base_location.is_null())
      {
        MGL_CORE_ERROR("load_program: path not found: {0}", path);
        return nullptr;
      }

      std::string shader_code;

      if(base_location.is_zip)
      {
        mgl::zip_ifsteam zip_file(base_location.path, path);
        std::string shader_text((std::istreambuf_iterator<char>(zip_file)),
                                std::istreambuf_iterator<char>());

        std::string data((std::istreambuf_iterator<char>(zip_file)),
                         std::istreambuf_iterator<char>());

        shader_code = data;
      }
      else
      {
        auto full_path = base_location.path / path;

        mgl::input_file shader_file(full_path, mgl::input_file::in);
        std::string data((std::istreambuf_iterator<char>(shader_file)),
                         std::istreambuf_iterator<char>());

        shader_code = data;
      }

      mgl::opengl::shader shader_source(shader_code);
      mgl::opengl::shaders glsl = { shader_source };
      mgl::opengl::shaders_outputs outputs = opts.outputs;

      if(outputs.size() == 0 && glsl.fragment().empty())
      {
        MGL_CORE_ASSERT(!glsl.vertex().empty() || !glsl.geometry().empty(),
                        "Required vertex or geometry shader");
        outputs = shader_source.outputs();
      }

      return ctx->program(glsl, outputs);
    }

    mgl::window::api::program_ref load_program(mgl::opengl::shader& source,
                                               const program_load_opts& opts)
    {
      MGL_CORE_ASSERT(mgl::window::current_context(), "No context!");
      const auto ctx = mgl::window::current_context();

      mgl::opengl::shaders glsl = { source };

      MGL_CORE_ASSERT(!glsl.empty(), "Empty source");
      mgl::opengl::shaders_outputs outputs = opts.outputs;

      return ctx->program(glsl, outputs);
    }

  } // namespace resources
} // namespace  mgl::window