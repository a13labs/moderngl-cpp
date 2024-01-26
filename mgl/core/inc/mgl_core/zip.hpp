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
  /**
   * Represents a file within a ZIP archive.
   */
  class zip_file;
  using zip_file_ref = mgl::ref<zip_file>;

  class zip_ifstream;
  using zip_ifstream_ref = mgl::ref<zip_ifstream>;

  class zip_file
  {
public:
    /**
     * Constructs a zip_file object with the specified source.
     * 
     * @param source The path to the ZIP archive.
     */
    zip_file(const std::string& source);

    /**
     * Destructor.
     */
    ~zip_file() = default;

    /**
     * Checks if a file with the specified path exists within the ZIP archive.
     * 
     * @param path The path of the file to check.
     * @return True if the file exists, false otherwise.
     */
    bool exists(const mgl::path& path) const;

    /**
     * Checks if the zip_file object is valid.
     * 
     * @return True if the zip_file object is valid, false otherwise.
     */
    bool is_valid() const { return !m_source.empty(); }

    /**
     * Reads the contents of a file within the ZIP archive into a byte buffer.
     * 
     * @param path The path of the file to read.
     * @param buffer The byte buffer to store the file contents.
     */
    void read(const mgl::path& path, mgl::uint8_buffer& buffer) const;

    zip_ifstream_ref open(const mgl::path& path) const;

    /**
     * Checks if a file with the specified source is a valid ZIP file.
     * 
     * @param source The path to the file to check.
     * @return True if the file is a valid ZIP file, false otherwise.
     */
    static bool is_zip_file(const std::string& source);

private:
    std::string m_source;
  };

  /**
   * Represents an input stream for reading files within a ZIP archive.
   */
  class zip_ifstream : public std::istream
  {
public:
    /**
     * Constructs a zip_ifstream object with the specified source and filename.
     * 
     * @param source The path to the ZIP archive.
     * @param filename The name of the file within the ZIP archive to read.
     */
    zip_ifstream(const std::string& source, const std::string& filename);

    /**
     * Destructor.
     */
    ~zip_ifstream();

private:
    static constexpr std::streamsize BUFFER_SIZE = 1024;
    char m_buffer_data[BUFFER_SIZE];
    zip_t* m_zip_file = nullptr;
    zip_file_t* m_zip_entry = nullptr;

    /**
     * Represents a buffer for reading data from a zip_file.
     */
    class zip_file_buffer : public std::streambuf
    {
  public:
      zip_file_buffer() = default;

      /**
       * Reads the next character from the buffer.
       * 
       * @return The next character, or EOF if the end of the buffer is reached.
       */
      int_type underflow() override;

      /**
       * Sets the zip_file entry for the buffer.
       * 
       * @param entry The zip_file entry to set.
       */
      void set_zip_entry(zip_file_t* entry) { m_zip_entry = entry; }

  private:
      char m_buffer[BUFFER_SIZE];
      zip_file_t* m_zip_entry = nullptr;
    };

    zip_file_buffer m_buffer;
  };

} // namespace mgl