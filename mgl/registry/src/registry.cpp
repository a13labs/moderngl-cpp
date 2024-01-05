#include "mgl_registry/registry.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_core/zip.hpp"

namespace mgl::registry
{
  location null_location = { mgl::null_path, false, false };

  static registry* s_registry = nullptr;

  static const std::string types_names[] = { "image", "shader", "font",   "sound", "music",
                                             "text",  "json",   "binary", "custom" };

  registry::registry()
  {
    MGL_CORE_ASSERT(s_registry == nullptr, "Registry already exists");
    if(s_registry == nullptr)
      s_registry = this;

    {
      // Register loaders
      loader_ref image_loader = mgl::create_scope<loaders::image_loader>();
      register_loader(image_loader);

      loader_ref shader_loader = mgl::create_scope<loaders::shader_loader>();
      register_loader(shader_loader);

      loader_ref text_loader = mgl::create_scope<loaders::text_loader>();
      register_loader(text_loader);
    }
  }

  registry::~registry()
  {
    s_registry = nullptr;
    m_loaders.clear();
    m_locations.clear();
  }

  bool registry::register_dir(resource::type type, const std::string& path)
  {
    auto p = mgl::path(path);

    if(p.is_relative())
    {
      p = std::filesystem::current_path() / path;
    }

    if(!std::filesystem::exists(p))
    {
      MGL_CORE_ERROR("registry: path does not exist: {0}", path);
      return false;
    }

    bool is_zip = false;
    if(std::filesystem::is_regular_file(path))
    {
      // Must be a zip file or it is not supported
      if(!mgl::zip_file::is_zip_file(path))
      {
        MGL_CORE_ERROR("registry: path is not a zip file: {0}", path);
        return false;
      }
      is_zip = true;
    }
    else if(!std::filesystem::is_directory(path))
    {
      MGL_CORE_ERROR("registry: path is not a directory: {0}", path);
      return false;
    }

    if(m_locations.find(type) == m_locations.end())
      m_locations[type] = locations();

    auto& locations = m_locations[type];

    auto it = std::find(locations.begin(), locations.end(), (location){ path, is_zip });

    if(it == locations.end())
      locations.push_back({ path, is_zip });

    return true;
  }

  bool registry::register_loader(loader_ref& loader)
  {
    string_list extensions = loader->get_extensions();
    string_list extensions_to_handle;

    for(const auto& extension : extensions)
    {
      if(m_loaders.find(extension) != m_loaders.end())
      {
        MGL_CORE_ERROR("Loader already registered for extension: {}, skipping", extension);
        continue;
      }

      if(mgl::in(extension, extensions_to_handle))
      {
        MGL_CORE_ERROR("Extension already registered for handling: {}, skipping", extension);
        continue;
      }

      extensions_to_handle.push_back(extension);
    }

    if(extensions_to_handle.empty())
    {
      MGL_CORE_ERROR("No extensions to handle");
      return false;
    }

    loader_info_ref info = mgl::create_ref<loader_info>();
    info->loader = std::move(loader);

    for(const auto& extension : extensions_to_handle)
    {
      m_loaders[extension] = info;
    }

    return true;
  }

  const location& registry::find(resource::type type, const std::string& path)
  {
    auto p = mgl::path(path);

    if(m_locations.find(type) == m_locations.end())
    {
      MGL_CORE_ERROR("No locations registered for type: {}", types_names[(int)type]);
      return null_location;
    }

    auto& locations = m_locations[type];

    for(auto&& base : locations)
    {
      if(base.is_compressed)
      {
        mgl::zip_file zip(base.path);
        if(!zip.exists(p))
        {
          continue;
        }
        return base;
      }

      auto absolute_path = mgl::path(base.path) / path;

      if(std::filesystem::exists(absolute_path))
      {
        return base;
      }
    }

    return null_location;
  }

  resource_ref
  registry::load(resource::type type, const std::string& path, const loader_options& options)
  {
    auto& location = find(type, path);

    if(location.path == mgl::null_path)
    {
      MGL_CORE_ERROR("Failed to find resource: {}", path);
      return nullptr;
    }

    auto extension = mgl::path(path).extension().string();

    if(m_loaders.find(extension) == m_loaders.end())
    {
      MGL_CORE_ERROR("No loader registered for extension: {}", extension);
      return nullptr;
    }

    auto& loader = m_loaders[extension];
    return loader->loader->load(location, path, options);
  }

  bool registry::exists(const std::string& path) const
  {
    if(!std::filesystem::exists(path))
      return false;

    if(!std::filesystem::is_regular_file(path))
      return false;

    for(const auto& loader : m_loaders)
    {
      if(!in(path, loader.second->loader->get_extensions()))
        continue;

      return true;
    }

    return false;
  }

  registry& registry::current_registry()
  {
    MGL_CORE_ASSERT(s_registry != nullptr, "Registry does not exist");
    return *s_registry;
  }

} // namespace mgl::registry