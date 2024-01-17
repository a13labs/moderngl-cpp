#pragma once

#include "io.hpp"
#include "memory.hpp"
#include "string.hpp"

struct zip;
struct zip_file;
typedef struct zip zip_t;
typedef struct zip_file zip_file_t;

namespace mgl
{
  class zip_file
  {
public:
    zip_file(const std::string& source);

    ~zip_file() = default;

    bool exists(const mgl::path& path) const;

    bool is_valid() const { return !m_source.empty(); }

    void read(const mgl::path& path, mgl::buffer<uint8_t>& buffer) const;

    static bool is_zip_file(const std::string& source);

private:
    std::string m_source;
  };

  class zip_ifsteam : public std::istream
  {
public:
    zip_ifsteam(const std::string& source, const std::string& filename);
    ~zip_ifsteam();

private:
    static constexpr std::streamsize BUFFER_SIZE = 1024;
    char m_buffer_data[BUFFER_SIZE];
    zip_t* m_zip_file = nullptr;
    zip_file_t* m_zip_entry = nullptr;

    class zip_file_buffer : public std::streambuf
    {
  public:
      zip_file_buffer() = default;
      int_type underflow() override;
      void set_zip_entry(zip_file_t* entry) { m_zip_entry = entry; }

  private:
      char m_buffer[BUFFER_SIZE];
      zip_file_t* m_zip_entry = nullptr;
    };

    zip_file_buffer m_buffer;
  };

  using zip_ifsteam_ptr = mgl::ref<zip_ifsteam>;

} // namespace mgl