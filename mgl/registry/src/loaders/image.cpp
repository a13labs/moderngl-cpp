#include "mgl_registry/loaders/image.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/zip.hpp"

#include "mgl_core/log.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.hpp"

namespace mgl::registry::loaders
{
  static image_loader_options default_image_loader_options;

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
  mgl::string_list image_loader::get_extensions() const
  {
    return { ".png", ".jpg", ".jpeg", ".bmp", ".tga", ".psd", ".gif", ".hdr", ".pic" };
  }

  resource_ref image_loader::load(const location& location,
                                  const std::string& path,
                                  const loader_options& options)
  {
    const image_loader_options* opts = dynamic_cast<const image_loader_options*>(&options);

    if(!opts)
    {
      opts = &default_image_loader_options;
    }

    int width, height, components;
    stbi_set_flip_vertically_on_load(opts->flip_vertically);

    unsigned char* data = nullptr;

    if(location.is_compressed)
    {
      mgl::zip_file zip(location.path);
      if(!zip.exists(path))
      {
        MGL_CORE_ERROR("Failed to read image file: {}", path);
        return nullptr;
      }
      mgl::buffer<uint8_t> buffer;
      zip.read(path, buffer);
      data = stbi_load_from_memory(buffer.data(), buffer.size(), &width, &height, &components, 0);
    }
    else
    {
      auto full_path = mgl::path(location.path) / path;
      if(!std::filesystem::exists(full_path))
      {
        MGL_CORE_ERROR("Failed to read image file: {}", path);
        return nullptr;
      }
      data = stbi_load((const char*)full_path.c_str(), &width, &height, &components, 0);
    }

    if(opts->flip_vertically)
    {
      horizontal_flip(data, width, height, components);
    }

    auto img_data = mgl::buffer<uint8_t>(data, data + width * height * components);

    stbi_image_free(data);

    return mgl::create_ref<image>(width, height, components, img_data);
  }

} // namespace mgl::registry::loaders