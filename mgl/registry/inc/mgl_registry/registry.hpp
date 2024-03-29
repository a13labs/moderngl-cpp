#pragma once
#include "loader.hpp"
#include "loaders/image.hpp"
#include "loaders/music.hpp"
#include "loaders/palette.hpp"
#include "loaders/shader.hpp"
#include "loaders/sound.hpp"
#include "loaders/text.hpp"
#include "loaders/truetype.hpp"
#include "location.hpp"
#include "resource.hpp"
#include "resources/font.hpp"
#include "resources/image.hpp"
#include "resources/music.hpp"
#include "resources/palette.hpp"
#include "resources/shader.hpp"
#include "resources/sound.hpp"
#include "resources/text.hpp"

#include "mgl_core/io.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"

namespace mgl::registry
{
  class registry
  {
public:
    ~registry();

    bool register_location_factory(location_factory_ref& factory);
    bool register_location(resource::type type, const url& url);
    bool register_loader(loader_ref& loader);

    resource_ref load(resource::type type, const std::string& path, const loader_options& options);
    const location_ref find(resource::type type, const std::string& path);

    bool exists(const std::string& path) const;

    static registry& current_registry()
    {
      static registry s_registry;
      return s_registry;
    }

private:
    registry();

    struct loader_info
    {
      loader_ref loader;
    };

    struct location_factory_info
    {
      location_factory_ref factory;
    };

    using loader_info_ref = mgl::ref<loader_info>;
    using location_factory_info_ref = mgl::ref<location_factory_info>;

    mgl::unordered_map<std::string, loader_info_ref> m_loaders;
    mgl::unordered_map<std::string, location_factory_info_ref> m_locations_factories;
    mgl::unordered_map<resource::type, locations> m_locations;
  };

  using registry_ref = mgl::scope<registry>;

  inline registry& current_registry()
  {
    return registry::current_registry();
  }

  inline resource_ref
  load(resource::type type, const std::string& path, const loader_options& options)
  {
    return current_registry().load(type, path, options);
  }

  inline image_ref load_image(const std::string& path, const loader_options& options)
  {
    return std::dynamic_pointer_cast<image>(
        current_registry().load(resource::type::image, path, options));
  }

  inline shader_ref load_shader(const std::string& path, const loader_options& options)
  {
    return std::dynamic_pointer_cast<shader>(
        current_registry().load(resource::type::shader, path, options));
  }

  inline text_ref load_text(const std::string& path, const loader_options& options)
  {
    return std::dynamic_pointer_cast<text>(
        current_registry().load(resource::type::text, path, options));
  }

  inline font_ref load_font(const std::string& path, const loader_options& options)
  {
    return std::dynamic_pointer_cast<font>(
        current_registry().load(resource::type::font, path, options));
  }

  inline sound_ref load_sound(const std::string& path, const loader_options& options)
  {
    return std::dynamic_pointer_cast<sound>(
        current_registry().load(resource::type::sound, path, options));
  }

  inline music_ref load_music(const std::string& path, const loader_options& options)
  {
    return std::dynamic_pointer_cast<music>(
        current_registry().load(resource::type::music, path, options));
  }

  inline const location_ref find(resource::type type, const std::string& path)
  {
    return current_registry().find(type, path);
  }

  inline bool register_location(resource::type type, const std::string& path)
  {
    return current_registry().register_location(type, path);
  }

  inline bool register_loader(loader_ref& loader)
  {
    return current_registry().register_loader(loader);
  }

  inline bool register_location_factory(location_factory_ref& factory)
  {
    return current_registry().register_location_factory(factory);
  }

} // namespace mgl::registry
