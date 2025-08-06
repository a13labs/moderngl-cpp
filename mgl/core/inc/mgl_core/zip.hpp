#pragma once

#include "io.hpp"
#include "memory.hpp"
#include "string.hpp"
#include "utils.hpp"

/**
 * \brief This is the main namespace of the MGL library, encapsulating all core functionalities and utilities.
 */
struct zip;
/**
 * \brief Represents a file within a ZIP archive.
 *
 * The `zip_file` class provides an abstraction for working with files in a ZIP archive. It allows reading and writing of data, similar to the standard C++ file streams.
 * This makes it easy to work with compressed archives without having to deal directly with the complexities of handling ZIP files.
 */
struct zip_file;
/**
 * \brief A type representing a ZIP archive file.
 *
 * This is an abstract representation of a ZIP archive file, providing high-level operations for reading from and writing to the archive.
 * It allows developers to work with ZIP archives in a structured way, abstracting away complexities related to handling ZIP files directly.
 */
typedef struct zip zip_t;
/**
 * \brief Represents a file within a ZIP archive.
 *
 * The `zip_file_t` represents an individual file within a ZIP archive. It provides functions to read from and write to the file, as well as getters for information about the file such as its name and size. 
 */
typedef struct zip_file zip_file_t;

/**
 * \brief The mgl namespace encapsulates the core functionalities of the MGL library, providing a structured and organized way to access these utilities.
 * It includes sub-namespaces such as io (input/output), log (logging) and profiling (profiling and instrumentation). 
 * These namespaces provide specific functionalities for managing file operations, logging messages at different levels, and measuring the execution time of code blocks or functions respectively.
 */
namespace mgl
{
  /**
   * Represents a file within a ZIP archive.
   */
  class zip_file;
/**
 * \brief A reference to an instance of the `zip_file` class in the MGL library.
 *
 * This type is used as a shorthand for referring to instances of the `zip_file` class, which represents files within a ZIP archive. 
 * It provides a safer and more convenient way to handle file operations related to ZIP archives compared to directly using pointers or references to `zip_file` objects.
 */
  using zip_file_ref = mgl::ref<zip_file>;

/**
 * \brief A class that represents an input stream from a file within a ZIP archive.
 *
 * This class provides the functionality to read data from a specified position in a ZIP archive, similar to how an ifstream reads from a file. It allows for reading various data types directly from files within a ZIP archive, abstracting away the complexities of handling ZIP files directly.
 * 
 * \note This class is part of the MGL (Micro Game Library) and provides input stream functionalities related to working with ZIP archives. It's designed to offer a high-level interface for reading from ZIP archives, abstracting away the complexities of handling ZIP files directly.
 * 
 */
  class zip_ifstream;
/**
 * \brief A reference to an input stream associated with a file within a ZIP archive.
 *
 * This type is used as a shorthand for managing the lifecycle of an `mgl::io::zip_ifstream` object, which represents a file 
 * within a ZIP archive and provides methods for reading data from that file. It's essentially a reference to an instance 
 * of `mgl::ref<zip_ifstream>`, providing a way to manage the lifetime of this stream without directly managing its memory.
 */
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

/**
 * \brief Opens a file within the ZIP archive as an input stream reference.
 *
 * This function opens a file from the ZIP archive specified by `path` and returns it as an input stream reference. The returned object can be used to read data from the file.
 * 
 * \param path The path of the file within the ZIP archive to open.
 * \return An input stream reference to the opened file.
 * 
 * \throws std::runtime_error If the file could not be opened for any reason, a `std::runtime_error` is thrown with an appropriate error message.
 */
    zip_ifstream_ref open(const mgl::path& path) const;

    /**
     * Checks if a file with the specified source is a valid ZIP file.
     * 
     * @param source The path to the file to check.
     * @return True if the file is a valid ZIP file, false otherwise.
     */
    static bool is_zip_file(const std::string& source);

private:
/**
 * @brief A string variable that holds the source of data or file path.
 */
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

/**
 * @brief Represents a buffer of data stored in a ZIP archive file.
 * 
 * The `m_buffer` is an instance of the class `zip_file_buffer`, which provides an abstraction for working with files within a ZIP archive. It represents a buffer that holds data from a specific file within the ZIP archive. This buffer can be used to read or write data from/to this file in the ZIP archive.
 */
    zip_file_buffer m_buffer;
  };

} // namespace mgl