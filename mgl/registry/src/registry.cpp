#include "mgl_registry/registry.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_core/zip.hpp"
#include "mgl_registry/locations/local.hpp"
#include "mgl_registry/locations/zip.hpp"

namespace mgl::registry
{
  static registry* s_registry = nullptr;

  static const std::string types_names[] = { "image", "shader", "font",   "sound", "music",
                                             "text",  "json",   "binary", "custom" };

  registry::registry()
  {
    MGL_CORE_ASSERT(s_registry == nullptr, "Registry already exists");
    if(s_registry == nullptr)
      s_registry = this;

    {
      // Register location factories
      location_factory_ref local_factory = mgl::create_scope<local_location>();
      register_location_factory(local_factory);

      location_factory_ref zip_factory = mgl::create_scope<zip_location>();
      register_location_factory(zip_factory);

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

  bool registry::register_location_factory(location_factory_ref& factory)
  {
    if(factory == nullptr)
    {
      MGL_CORE_ERROR("registry: factory is null");
      return false;
    }

    if(m_locations_factories.find(factory->kind()) != m_locations_factories.end())
    {
      MGL_CORE_ERROR("registry: factory for kind '{}' already registered", factory->kind());
      return false;
    }

    location_factory_info_ref info = mgl::create_ref<location_factory_info>();
    info->factory = std::move(factory);

    m_locations_factories[info->factory->kind()] = info;
    return true;
  }

  bool registry::register_location(resource::type type, const url& url)
  {
    for(auto& [name, factory] : m_locations_factories)
    {
      auto& f = factory->factory;

      if(!f->can_handle(url))
        continue;

      auto location = f->factory(url);

      if(m_locations.find(type) == m_locations.end())
        m_locations[type] = locations();

      auto& locations = m_locations[type];

      for(auto&& l : locations)
      {
        if(l->path() == location->path())
        {
          MGL_CORE_ERROR("Location already registered: {}", location->path().string());
          return false;
        }
      }

      locations.push_back(location);
      return true;
    }

    MGL_CORE_ERROR("No location factory registered for url: {}", url.path.string());
    return false;
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

  const location_ref registry::find(resource::type type, const std::string& path)
  {
    if(m_locations.find(type) == m_locations.end())
    {
      MGL_CORE_ERROR("No locations registered for type: {}", types_names[(int)type]);
      return nullptr;
    }

    auto& locations = m_locations[type];

    for(auto&& base : locations)
    {
      if(base->exists(path))
      {
        return base;
      }
    }

    return nullptr;
  }

  resource_ref
  registry::load(resource::type type, const std::string& path, const loader_options& options)
  {
    auto& location = find(type, path);

    if(location == nullptr)
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
    for(auto&& [type, locations] : m_locations)
    {
      for(auto&& location : locations)
      {
        if(location->exists(path))
          return true;
      }
    }

    return false;
  }

  registry& registry::current_registry()
  {
    MGL_CORE_ASSERT(s_registry != nullptr, "Registry does not exist");
    return *s_registry;
  }

} // namespace mgl::registry