#include "mgl_core/zip.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_core/io.hpp"
#include "mgl_core/log.hpp"

namespace mgl
{

  zip_file::zip_file(const std::string& source)
      : m_source(source)
  {
    if(!std::filesystem::exists(source))
    {
      MGL_CORE_ERROR("zip_file: path does not exist: {0}", source);
      m_source = "";
    }
  }

  bool zip_file::exists(const mgl::path& path) const
  {
    if(m_source.empty())
    {
      MGL_CORE_ERROR("zip_file: source is empty");
      return false;
    }

    auto z_file = zip_open(m_source.c_str(), 0, nullptr);

    if(!z_file)
    {
      MGL_CORE_ERROR("zip_file: error opening zip file: {0}", m_source);
      return false;
    }

    zip_stat_t z_stat;
    auto err = zip_stat(z_file, path.string().c_str(), 0, &z_stat);

    zip_close(z_file);

    if(err != 0)
    {
      return false;
    }

    return true;
  }

  void zip_file::read(const mgl::path& path, mgl::buffer<uint8_t>& buffer) const
  {
    if(m_source.empty())
    {
      MGL_CORE_ERROR("zip_file: source is empty");
      return;
    }

    auto z_file = zip_open(m_source.c_str(), 0, nullptr);

    if(!z_file)
    {
      MGL_CORE_ERROR("zip_file: error opening zip file: {0}", m_source);
      return;
    }

    zip_stat_t z_stat;

    auto err = zip_stat(z_file, path.string().c_str(), 0, &z_stat);

    if(err != 0)
    {
      zip_close(z_file);
      return;
    }

    auto z_entry = zip_fopen(z_file, path.string().c_str(), 0);

    if(!z_entry)
    {
      zip_close(z_file);
      return;
    }

    buffer.resize(z_stat.size);

    auto bytes_read = zip_fread(z_entry, buffer.data(), z_stat.size);

    if(bytes_read == -1)
    {
      zip_fclose(z_entry);
      zip_close(z_file);
      return;
    }

    zip_fclose(z_entry);
    zip_close(z_file);
  }

  bool zip_file::is_zip_file(const std::string& source)
  {
    auto z_file = zip_open(source.c_str(), 0, nullptr);

    if(!z_file)
    {
      return false;
    }

    zip_close(z_file);
    return true;
  }

  zip_ifsteam::zip_ifsteam(const std::string& source, const std::string& filename)
      : std::istream(&m_buffer)
  {
    m_zip_file = zip_open(source.c_str(), 0, nullptr);

    if(!m_zip_file)
    {
      MGL_CORE_ERROR("zip_ifsteam: error opening zip file: {0}", source);
      setstate(std::ios_base::failbit);
      return;
    }

    m_zip_entry = zip_fopen(m_zip_file, filename.c_str(), 0);

    if(!m_zip_entry)
    {
      MGL_CORE_ERROR("zip_ifsteam: error opening zip entry: {0}", filename);
      setstate(std::ios_base::failbit);
      zip_close(m_zip_file);
      return;
    }

    m_buffer.pubsetbuf(m_buffer_data, BUFFER_SIZE);
    m_buffer.pubseekpos(0, std::ios_base::in);
    m_buffer.set_zip_entry(m_zip_entry);
  }

  zip_ifsteam::~zip_ifsteam()
  {
    if(m_zip_entry)
    {
      zip_fclose(m_zip_entry);
    }

    if(m_zip_file)
    {
      zip_close(m_zip_file);
    }
  }

  std::streambuf::int_type zip_ifsteam::zip_file_buffer::underflow()
  {
    if(gptr() < egptr())
    {
      return traits_type::to_int_type(*gptr());
    }

    auto bytes_read = zip_fread(m_zip_entry, m_buffer, BUFFER_SIZE);

    if(bytes_read <= 0)
    {
      return traits_type::eof();
    }

    setg(m_buffer, m_buffer, m_buffer + bytes_read);

    return traits_type::to_int_type(*gptr());
  }

} // namespace mgl
