/*
   Copyright 2022 Alexandre Pires (c.alexandre.pires@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#pragma once

#include "containers.hpp"
#include "debug.hpp"
#include "memory.hpp"
#include "utils.hpp"
#include <fstream>

#include "glm/glm.hpp"

/**
 * @file io.hpp
 * @brief Contains the definition of the mgl namespace and related types for core input/output functionality.
 */
/**
  * @brief Namespace for core input/output functionality.
  */
namespace mgl::io
{
  /**
   * @brief Alias for std::istream.
   */
  using istream = std::istream;

  /**
   * @brief Alias for std::ofstream.
   */
  using ostream = std::ostream;

  /**
   * @brief Alias for std::ifstream.
   */
  using ifstream = std::ifstream;

  /**
   * @brief Alias for std::ofstream.
   */
  using ofstream = std::ofstream;

  /**
   * @brief Alias for mgl::ref<std::ifstream>.
   */
  using istream_ref = mgl::ref<istream>;

  /**
   * @brief Alias for mgl::ref<std::ofstream>.
   */
  using ostream_ref = mgl::ref<ostream>;

  /**
   * @brief Alias for mgl::ref<ifstream>.
   */
  using ifstream_ref = mgl::ref<ifstream>;

  /**
   * @brief Alias for mgl::ref<ofstream>.
   */
  using ofstream_ref = mgl::ref<ofstream>;

  /**
   * @brief Constant representing a null path.
   */
  extern const path null_path;

  /**
   * @brief Alias to std::ios_base::openmode.
   */
  using openmode = std::ios_base::openmode;

  static const openmode binary = std::ios_base::binary;
  static const openmode in = std::ios_base::in;
  static const openmode out = std::ios_base::out;
  static const openmode trunc = std::ios_base::trunc;

  // Reading functions

  /**
   * @brief reads a null terminated string from a file.
   * @param file The file to read from.
   */
  inline std::string read_string(const istream_ref& file)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    std::string result;
    char c;
    while(file->read(&c, 1) && c != '\0')
      result += c;
    return result;
  }

  /**
   * @brief read bool from a file.
   * @param file The file to read from.
   */
  inline bool read_bool(const istream_ref& file)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_bool: file is not open");
    bool result;
    file->read(reinterpret_cast<char*>(&result), sizeof(bool));
    return result;
  }

  /**
   * @brief reads a uint8_t from a file.
   * @param file The file to read from.
   */
  inline uint8_t read_uint8(const istream_ref& file)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    uint8_t result;
    file->read(reinterpret_cast<char*>(&result), sizeof(uint8_t));
    return result;
  }

  /**
   * @brief reads a uint16_t from a file.
   * @param file The file to read from.
   */
  inline uint16_t read_uint16(const istream_ref& file)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    uint16_t result;
    file->read(reinterpret_cast<char*>(&result), sizeof(uint16_t));
    return result;
  }

  /**
   * @brief reads a uint32_t from a file.
   * @param file The file to read from.
   */
  inline uint32_t read_uint32(const istream_ref& file)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    uint32_t result;
    file->read(reinterpret_cast<char*>(&result), sizeof(uint32_t));
    return result;
  }

  /**
   * @brief reads a uint64_t from a file.
   * @param file The file to read from.
   */
  inline uint64_t read_uint64(const istream_ref& file)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    uint64_t result;
    file->read(reinterpret_cast<char*>(&result), sizeof(uint64_t));
    return result;
  }

  /**
   * @brief reads a int8_t from a file.
   * @param file The file to read from.
   */
  inline int8_t read_int8(const istream_ref& file)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    int8_t result;
    file->read(reinterpret_cast<char*>(&result), sizeof(int8_t));
    return result;
  }

  /**
   * @brief reads a int16_t from a file.
   * @param file The file to read from.
   */
  inline int16_t read_int16(const istream_ref& file)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    int16_t result;
    file->read(reinterpret_cast<char*>(&result), sizeof(int16_t));
    return result;
  }

  /**
   * @brief reads a int32_t from a file.
   * @param file The file to read from.
   */
  inline int32_t read_int32(const istream_ref& file)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    int32_t result;
    file->read(reinterpret_cast<char*>(&result), sizeof(int32_t));
    return result;
  }

  /**
   * @brief reads a int64_t from a file.
   * @param file The file to read from.
   */
  inline int64_t read_int64(const istream_ref& file)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    int64_t result;
    file->read(reinterpret_cast<char*>(&result), sizeof(int64_t));
    return result;
  }

  /**
   * @brief reads a float from a file.
   * @param file The file to read from.
   */
  inline float read_float32(const istream_ref& file)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    float result;
    file->read(reinterpret_cast<char*>(&result), sizeof(float));
    return result;
  }

  /**
   * @brief reads a double from a file.
   * @param file The file to read from.
   */
  inline double read_float64(const istream_ref& file)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    double result;
    file->read(reinterpret_cast<char*>(&result), sizeof(double));
    return result;
  }

  /**
   * @brief reads a glm::vec2 from a file.
   * @param file The file to read from.
   */
  inline glm::vec2 read_vec2(const istream_ref& file)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    glm::vec2 result;
    file->read(reinterpret_cast<char*>(&result), sizeof(glm::vec2));
    return result;
  }

  /**
   * @brief reads a glm::vec3 from a file.
   * @param file The file to read from.
   */
  inline glm::vec3 read_vec3(const istream_ref& file)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    glm::vec3 result;
    file->read(reinterpret_cast<char*>(&result), sizeof(glm::vec3));
    return result;
  }

  /**
   * @brief reads a glm::vec4 from a file.
   * @param file The file to read from.
   */
  inline glm::vec4 read_vec4(const istream_ref& file)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    glm::vec4 result;
    file->read(reinterpret_cast<char*>(&result), sizeof(glm::vec4));
    return result;
  }

  /**
   * @brief reads a glm::ivec2 from a file.
   * @param file The file to read from.
   */
  inline glm::ivec2 read_ivec2(const istream_ref& file)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    glm::ivec2 result;
    file->read(reinterpret_cast<char*>(&result), sizeof(glm::ivec2));
    return result;
  }

  /**
   * @brief reads a glm::ivec3 from a file.
   * @param file The file to read from.
   */
  inline glm::ivec3 read_ivec3(const istream_ref& file)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    glm::ivec3 result;
    file->read(reinterpret_cast<char*>(&result), sizeof(glm::ivec3));
    return result;
  }

  /**
   * @brief reads a glm::ivec4 from a file.
   * @param file The file to read from.
   */
  inline glm::ivec4 read_ivec4(const istream_ref& file)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    glm::ivec4 result;
    file->read(reinterpret_cast<char*>(&result), sizeof(glm::ivec4));
    return result;
  }

  /**
   * @brief reads a glm::mat2 from a file.
   * @param file The file to read from.
   */
  inline glm::mat2 read_mat2(const istream_ref& file)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    glm::mat2 result;
    file->read(reinterpret_cast<char*>(&result), sizeof(glm::mat2));
    return result;
  }

  /**
   * @brief reads a glm::mat3 from a file.
   * @param file The file to read from.
   */
  inline glm::mat3 read_mat3(const istream_ref& file)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    glm::mat3 result;
    file->read(reinterpret_cast<char*>(&result), sizeof(glm::mat3));
    return result;
  }

  /**
   * @brief reads a glm::mat4 from a file.
   * @param file The file to read from.
   */
  inline glm::mat4 read_mat4(const istream_ref& file)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    glm::mat4 result;
    file->read(reinterpret_cast<char*>(&result), sizeof(glm::mat4));
    return result;
  }

  /**
   * @brief reads a uint8 buffer from a file.
   * @param file The file to read from.
   * @param buffer The buffer to read into.
   */
  inline void read_uint8_buffer(const istream_ref& file, uint8_buffer& buffer)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    file->read(reinterpret_cast<char*>(buffer.data()), buffer.size());
  }

  /**
   * @brief reads a uint8 buffer from a file.
   * @param file The file to read from.
   * @param buffer The buffer to read into.
   * @param size The size of the buffer to read.
   * @note The size parameter is in number of elements, not bytes.
   */
  inline void
  read_uint8_buffer(const istream_ref& file, uint8_buffer& buffer, size_t size, size_t offset = 0)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    MGL_CORE_ASSERT(size <= buffer.size(), "read_bytes: size is greater than buffer size");
    MGL_CORE_ASSERT(offset + size <= buffer.size(),
                    "read_bytes: offset + size is greater than buffer size");
    file->read(reinterpret_cast<char*>(buffer.data() + offset), size);
  }

  /**
   * @brief reads a uint16 buffer from a file.
   * @param file The file to read from.
   * @param buffer The buffer to read into.
   */
  inline void read_uint16_buffer(const istream_ref& file, uint16_buffer& buffer)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    file->read(reinterpret_cast<char*>(buffer.data()), buffer.size() * sizeof(uint16_t));
  }

  /**
   * @brief reads a uint16 buffer from a file.
   * @param file The file to read from.
   * @param buffer The buffer to read into.
   * @param size The size of the buffer to read.
   * @note The size parameter is in number of elements, not bytes.
   */
  inline void
  read_uint16_buffer(const istream_ref& file, uint16_buffer& buffer, size_t size, size_t offset = 0)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    MGL_CORE_ASSERT(size <= buffer.size(), "read_bytes: size is greater than buffer size");
    MGL_CORE_ASSERT(offset + size <= buffer.size(),
                    "read_bytes: offset + size is greater than buffer size");
    file->read(reinterpret_cast<char*>(buffer.data() + offset), size * sizeof(uint16_t));
  }

  /**
   * @brief reads a uint32 buffer from a file.
   * @param file The file to read from.
   * @param buffer The buffer to read into.
   */
  inline void read_uint32_buffer(const istream_ref& file, uint32_buffer& buffer)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    file->read(reinterpret_cast<char*>(buffer.data()), buffer.size() * sizeof(uint32_t));
  }

  /**
   * @brief reads a uint32 buffer from a file.
   * @param file The file to read from.
   * @param buffer The buffer to read into.
   * @param size The size of the buffer to read.
   * @note The size parameter is in number of elements, not bytes.
   */
  inline void
  read_uint32_buffer(const istream_ref& file, uint32_buffer& buffer, size_t size, size_t offset = 0)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    MGL_CORE_ASSERT(size <= buffer.size(), "read_bytes: size is greater than buffer size");
    MGL_CORE_ASSERT(offset + size <= buffer.size(),
                    "read_bytes: offset + size is greater than buffer size");
    file->read(reinterpret_cast<char*>(buffer.data() + offset), size * sizeof(uint32_t));
  }

  /**
   * @brief reads a uint64 buffer from a file.
   * @param file The file to read from.
   * @param buffer The buffer to read into.
   */
  inline void read_uint64_buffer(const istream_ref& file, uint64_buffer& buffer)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    file->read(reinterpret_cast<char*>(buffer.data()), buffer.size() * sizeof(uint64_t));
  }

  /**
   * @brief reads a uint64 buffer from a file.
   * @param file The file to read from.
   * @param buffer The buffer to read into.
   * @param size The size of the buffer to read.
   * @note The size parameter is in number of elements, not bytes.
   */
  inline void
  read_uint64_buffer(const istream_ref& file, uint64_buffer& buffer, size_t size, size_t offset = 0)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    MGL_CORE_ASSERT(size <= buffer.size(), "read_bytes: size is greater than buffer size");
    MGL_CORE_ASSERT(offset + size <= buffer.size(),
                    "read_bytes: offset + size is greater than buffer size");
    file->read(reinterpret_cast<char*>(buffer.data() + offset), size * sizeof(uint64_t));
  }

  /**
   * @brief reads a int8 buffer from a file.
   * @param file The file to read from.
   * @param buffer The buffer to read into.
   */
  inline void read_int8_buffer(const istream_ref& file, int8_buffer& buffer)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    file->read(reinterpret_cast<char*>(buffer.data()), buffer.size());
  }

  /**
   * @brief reads a int8 buffer from a file.
   * @param file The file to read from.
   * @param buffer The buffer to read into.
   * @param size The size of the buffer to read.
   * @note The size parameter is in number of elements, not bytes.
   */
  inline void
  read_int8_buffer(const istream_ref& file, int8_buffer& buffer, size_t size, size_t offset = 0)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    MGL_CORE_ASSERT(size <= buffer.size(), "read_bytes: size is greater than buffer size");
    MGL_CORE_ASSERT(offset + size <= buffer.size(),
                    "read_bytes: offset + size is greater than buffer size");
    file->read(reinterpret_cast<char*>(buffer.data() + offset), size);
  }

  /**
   * @brief reads a int16 buffer from a file.
   * @param file The file to read from.
   * @param buffer The buffer to read into.
   */
  inline void read_int16_buffer(const istream_ref& file, int16_buffer& buffer)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    file->read(reinterpret_cast<char*>(buffer.data()), buffer.size() * sizeof(int16_t));
  }

  /**
   * @brief reads a int16 buffer from a file.
   * @param file The file to read from.
   * @param buffer The buffer to read into.
   * @param size The size of the buffer to read.
   * @note The size parameter is in number of elements, not bytes.
   */
  inline void
  read_int16_buffer(const istream_ref& file, int16_buffer& buffer, size_t size, size_t offset = 0)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    MGL_CORE_ASSERT(size <= buffer.size(), "read_bytes: size is greater than buffer size");
    MGL_CORE_ASSERT(offset + size <= buffer.size(),
                    "read_bytes: offset + size is greater than buffer size");
    file->read(reinterpret_cast<char*>(buffer.data() + offset), size * sizeof(int16_t));
  }

  /**
   * @brief reads a int32 buffer from a file.
   * @param file The file to read from.
   * @param buffer The buffer to read into.
   */
  inline void read_int32_buffer(const istream_ref& file, int32_buffer& buffer)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    file->read(reinterpret_cast<char*>(buffer.data()), buffer.size() * sizeof(int32_t));
  }

  /**
   * @brief reads a int32 buffer from a file.
   * @param file The file to read from.
   * @param buffer The buffer to read into.
   * @param size The size of the buffer to read.
   * @note The size parameter is in number of elements, not bytes.
   */
  inline void
  read_int32_buffer(const istream_ref& file, int32_buffer& buffer, size_t size, size_t offset = 0)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    MGL_CORE_ASSERT(size <= buffer.size(), "read_bytes: size is greater than buffer size");
    MGL_CORE_ASSERT(offset + size <= buffer.size(),
                    "read_bytes: offset + size is greater than buffer size");
    file->read(reinterpret_cast<char*>(buffer.data() + offset), size * sizeof(int32_t));
  }

  /**
   * @brief reads a int64 buffer from a file.
   * @param file The file to read from.
   * @param buffer The buffer to read into.
   */
  inline void read_int64_buffer(const istream_ref& file, int64_buffer& buffer)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    file->read(reinterpret_cast<char*>(buffer.data()), buffer.size() * sizeof(int64_t));
  }

  /**
   * @brief reads a int64 buffer from a file.
   * @param file The file to read from.
   * @param buffer The buffer to read into.
   * @param size The size of the buffer to read.
   * @note The size parameter is in number of elements, not bytes.
   */
  inline void
  read_int64_buffer(const istream_ref& file, int64_buffer& buffer, size_t size, size_t offset = 0)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    MGL_CORE_ASSERT(size <= buffer.size(), "read_bytes: size is greater than buffer size");
    MGL_CORE_ASSERT(offset + size <= buffer.size(),
                    "read_bytes: offset + size is greater than buffer size");
    file->read(reinterpret_cast<char*>(buffer.data() + offset), size * sizeof(int64_t));
  }

  /**
   * @brief reads a float32 buffer from a file.
   * @param file The file to read from.
   * @param buffer The buffer to read into.
   */
  inline void read_float32_buffer(const istream_ref& file, float32_buffer& buffer)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    file->read(reinterpret_cast<char*>(buffer.data()), buffer.size() * sizeof(float));
  }

  /**
   * @brief reads a float32 buffer from a file.
   * @param file The file to read from.
   * @param buffer The buffer to read into.
   * @param size The size of the buffer to read.
   * @note The size parameter is in number of elements, not bytes.
   */
  inline void read_float32_buffer(const istream_ref& file,
                                  float32_buffer& buffer,
                                  size_t size,
                                  size_t offset = 0)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    MGL_CORE_ASSERT(size <= buffer.size(), "read_bytes: size is greater than buffer size");
    MGL_CORE_ASSERT(offset + size <= buffer.size(),
                    "read_bytes: offset + size is greater than buffer size");
    file->read(reinterpret_cast<char*>(buffer.data() + offset), size * sizeof(float));
  }

  /**
   * @brief reads a float64 buffer from a file.
   * @param file The file to read from.
   * @param buffer The buffer to read into.
   */
  inline void read_float64_buffer(const istream_ref& file, float64_buffer& buffer)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    file->read(reinterpret_cast<char*>(buffer.data()), buffer.size() * sizeof(double));
  }

  /**
   * @brief reads a float64 buffer from a file.
   * @param file The file to read from.
   * @param buffer The buffer to read into.
   * @param size The size of the buffer to read.
   * @note The size parameter is in number of elements, not bytes.
   */
  inline void read_float64_buffer(const istream_ref& file,
                                  float64_buffer& buffer,
                                  size_t size,
                                  size_t offset = 0)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_string: file is not open");
    MGL_CORE_ASSERT(size <= buffer.size(), "read_bytes: size is greater than buffer size");
    MGL_CORE_ASSERT(offset + size <= buffer.size(),
                    "read_bytes: offset + size is greater than buffer size");
    file->read(reinterpret_cast<char*>(buffer.data() + offset), size * sizeof(double));
  }

  /**
   * @brief read a text file into a string.
   * @param file The file to read from.
   */
  inline std::string read_text(const istream_ref& file)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "read_text: file is not open");
    std::string txt((std::istreambuf_iterator<char>(*file)), std::istreambuf_iterator<char>());
    return txt;
  }

  // Writing functions

  /**
   * @brief writes a null terminated string to a file.
   * @param file The file to write to.
   * @param str The string to write.
   */
  inline void write_string(const ofstream_ref& file, const std::string& str)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_string: file is not open");
    file->write(str.c_str(), str.size() + 1);
  }

  /**
   * @brief write bool to a file.
   * @param file The file to write to.
   * @param value The value to write.
   */
  inline void write_bool(const ofstream_ref& file, bool value)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_bool: file is not open");
    file->write(reinterpret_cast<char*>(&value), sizeof(bool));
  }

  /**
   * @brief writes a uint8_t to a file.
   * @param file The file to write to.
   * @param value The value to write.
   */
  inline void write_uint8(const ofstream_ref& file, uint8_t value)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_uint8: file is not open");
    file->write(reinterpret_cast<char*>(&value), sizeof(uint8_t));
  }

  /**
   * @brief writes a uint16_t to a file.
   * @param file The file to write to.
   * @param value The value to write.
   */
  inline void write_uint16(const ofstream_ref& file, uint16_t value)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_uint16: file is not open");
    file->write(reinterpret_cast<char*>(&value), sizeof(uint16_t));
  }

  /**
   * @brief writes a uint32_t to a file.
   * @param file The file to write to.
   * @param value The value to write.
   */
  inline void write_uint32(const ofstream_ref& file, uint32_t value)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_uint32: file is not open");
    file->write(reinterpret_cast<char*>(&value), sizeof(uint32_t));
  }

  /**
   * @brief writes a uint64_t to a file.
   * @param file The file to write to.
   * @param value The value to write.
   */
  inline void write_uint64(const ofstream_ref& file, uint64_t value)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_uint64: file is not open");
    file->write(reinterpret_cast<char*>(&value), sizeof(uint64_t));
  }

  /**
   * @brief writes a int8_t to a file.
   * @param file The file to write to.
   * @param value The value to write.
   */
  inline void write_int8(const ofstream_ref& file, int8_t value)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_int8: file is not open");
    file->write(reinterpret_cast<char*>(&value), sizeof(int8_t));
  }

  /**
   * @brief writes a int16_t to a file.
   * @param file The file to write to.
   * @param value The value to write.
   */
  inline void write_int16(const ofstream_ref& file, int16_t value)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_int16: file is not open");
    file->write(reinterpret_cast<char*>(&value), sizeof(int16_t));
  }

  /**
   * @brief writes a int32_t to a file.
   * @param file The file to write to.
   * @param value The value to write.
   */
  inline void write_int32(const ofstream_ref& file, int32_t value)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_int32: file is not open");
    file->write(reinterpret_cast<char*>(&value), sizeof(int32_t));
  }

  /*
    * @brief write int64_t to a file.
    * @param file The file to write to.
    * @param value The value to write.
    */
  inline void write_int64(const ofstream_ref& file, int64_t value)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_int64: file is not open");
    file->write(reinterpret_cast<char*>(&value), sizeof(int64_t));
  }

  /**
   * @brief writes a float to a file.
   * @param file The file to write to.
   * @param value The value to write.
   */
  inline void write_float32(const ofstream_ref& file, float value)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_float: file is not open");
    file->write(reinterpret_cast<char*>(&value), sizeof(float));
  }

  /**
   * @brief writes a double to a file.
   * @param file The file to write to.
   * @param value The value to write.
   */

  inline void write_float64(const ofstream_ref& file, double value)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_double: file is not open");
    file->write(reinterpret_cast<char*>(&value), sizeof(double));
  }

  /**
   * @brief writes a glm::vec2 to a file.
   * @param file The file to write to.
   * @param value The value to write.
   */

  inline void write_vec2(const ofstream_ref& file, const glm::vec2& value)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_vec2: file is not open");
    file->write(reinterpret_cast<const char*>(&value), sizeof(glm::vec2));
  }

  /**
   * @brief writes a glm::vec3 to a file.
   * @param file The file to write to.
   * @param value The value to write.
   */
  inline void write_vec3(const ofstream_ref& file, const glm::vec3& value)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_vec3: file is not open");
    file->write(reinterpret_cast<const char*>(&value), sizeof(glm::vec3));
  }

  /**
   * @brief writes a glm::vec4 to a file.
   * @param file The file to write to.
   * @param value The value to write.
   */
  inline void write_vec4(const ofstream_ref& file, const glm::vec4& value)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_vec4: file is not open");
    file->write(reinterpret_cast<const char*>(&value), sizeof(glm::vec4));
  }

  /**
   * @brief writes a glm::ivec2 to a file.
   * @param file The file to write to.
   * @param value The value to write.
   */
  inline void write_ivec2(const ofstream_ref& file, const glm::ivec2& value)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_ivec2: file is not open");
    file->write(reinterpret_cast<const char*>(&value), sizeof(glm::ivec2));
  }

  /**
   * @brief writes a glm::ivec3 to a file.
   * @param file The file to write to.
   * @param value The value to write.
   */
  inline void write_ivec3(const ofstream_ref& file, const glm::ivec3& value)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_ivec3: file is not open");
    file->write(reinterpret_cast<const char*>(&value), sizeof(glm::ivec3));
  }

  /**
   * @brief writes a glm::ivec4 to a file.
   * @param file The file to write to.
   * @param value The value to write.
   */
  inline void write_ivec4(const ofstream_ref& file, const glm::ivec4& value)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_ivec4: file is not open");
    file->write(reinterpret_cast<const char*>(&value), sizeof(glm::ivec4));
  }

  /**
   * @brief writes a glm::mat2 to a file.
   * @param file The file to write to.
   * @param value The value to write.
   */
  inline void write_mat2(const ofstream_ref& file, const glm::mat2& value)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_mat2: file is not open");
    file->write(reinterpret_cast<const char*>(&value), sizeof(glm::mat2));
  }

  /**
   * @brief writes a glm::mat3 to a file.
   * @param file The file to write to.
   * @param value The value to write.
   */
  inline void write_mat3(const ofstream_ref& file, const glm::mat3& value)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_mat3: file is not open");
    file->write(reinterpret_cast<const char*>(&value), sizeof(glm::mat3));
  }

  /**
   * @brief writes a glm::mat4 to a file.
   * @param file The file to write to.
   * @param value The value to write.
   */
  inline void write_mat4(const ofstream_ref& file, const glm::mat4& value)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_mat4: file is not open");
    file->write(reinterpret_cast<const char*>(&value), sizeof(glm::mat4));
  }

  /**
   * @brief writes a uint8 buffer to a file.
   * @param file The file to write to.
   * @param buffer The buffer to write.
   */
  inline void write_uint8_buffer(const ofstream_ref& file, const uint8_buffer& buffer)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_uint8_buffer: file is not open");
    file->write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
  }

  /**
   * @brief writes a uint8 buffer to a file.
   * @param file The file to write to.
   * @param buffer The buffer to write.
   * @param size The size of the buffer to write.
   * @note The size parameter is in number of elements, not bytes.
   */
  inline void write_uint8_buffer(const ofstream_ref& file,
                                 const uint8_buffer& buffer,
                                 size_t size,
                                 size_t offset = 0)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_uint8_buffer: file is not open");
    MGL_CORE_ASSERT(size <= buffer.size(), "write_bytes: size is greater than buffer size");
    MGL_CORE_ASSERT(offset + size <= buffer.size(),
                    "write_bytes: offset + size is greater than buffer size");
    file->write(reinterpret_cast<const char*>(buffer.data() + offset), size);
  }

  /**
   * @brief writes a uint16 buffer to a file.
   * @param file The file to write to.
   * @param buffer The buffer to write.
   */
  inline void write_uint16_buffer(const ofstream_ref& file, const uint16_buffer& buffer)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_uint16_buffer: file is not open");
    file->write(reinterpret_cast<const char*>(buffer.data()), buffer.size() * sizeof(uint16_t));
  }

  /**
   * @brief writes a uint16 buffer to a file.
   * @param file The file to write to.
   * @param buffer The buffer to write.
   * @param size The size of the buffer to write.
   * @note The size parameter is in number of elements, not bytes.
   */
  inline void write_uint16_buffer(const ofstream_ref& file,
                                  const uint16_buffer& buffer,
                                  size_t size,
                                  size_t offset = 0)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_uint16_buffer: file is not open");
    MGL_CORE_ASSERT(size <= buffer.size(), "write_bytes: size is greater than buffer size");
    MGL_CORE_ASSERT(offset + size <= buffer.size(),
                    "write_bytes: offset + size is greater than buffer size");
    file->write(reinterpret_cast<const char*>(buffer.data() + offset), size * sizeof(uint16_t));
  }

  /**
   * @brief writes a uint32 buffer to a file.
   * @param file The file to write to.
   * @param buffer The buffer to write.
   */
  inline void write_uint32_buffer(const ofstream_ref& file, const uint32_buffer& buffer)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_uint32_buffer: file is not open");
    file->write(reinterpret_cast<const char*>(buffer.data()), buffer.size() * sizeof(uint32_t));
  }

  /**
   * @brief writes a uint32 buffer to a file.
   * @param file The file to write to.
   * @param buffer The buffer to write.
   * @param size The size of the buffer to write.
   * @note The size parameter is in number of elements, not bytes.
   */
  inline void write_uint32_buffer(const ofstream_ref& file,
                                  const uint32_buffer& buffer,
                                  size_t size,
                                  size_t offset = 0)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_uint32_buffer: file is not open");
    MGL_CORE_ASSERT(size <= buffer.size(), "write_bytes: size is greater than buffer size");
    MGL_CORE_ASSERT(offset + size <= buffer.size(),
                    "write_bytes: offset + size is greater than buffer size");
    file->write(reinterpret_cast<const char*>(buffer.data() + offset), size * sizeof(uint32_t));
  }

  /**
   * @brief writes a uint64 buffer to a file.
   * @param file The file to write to.
   * @param buffer The buffer to write.
   */
  inline void write_uint64_buffer(const ofstream_ref& file, const uint64_buffer& buffer)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_uint64_buffer: file is not open");
    file->write(reinterpret_cast<const char*>(buffer.data()), buffer.size() * sizeof(uint64_t));
  }

  /**
   * @brief writes a uint64 buffer to a file.
   * @param file The file to write to.
   * @param buffer The buffer to write.
   * @param size The size of the buffer to write.
   * @note The size parameter is in number of elements, not bytes.
   */
  inline void write_uint64_buffer(const ofstream_ref& file,
                                  const uint64_buffer& buffer,
                                  size_t size,
                                  size_t offset = 0)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_uint64_buffer: file is not open");
    MGL_CORE_ASSERT(size <= buffer.size(), "write_bytes: size is greater than buffer size");
    MGL_CORE_ASSERT(offset + size <= buffer.size(),
                    "write_bytes: offset + size is greater than buffer size");
    file->write(reinterpret_cast<const char*>(buffer.data() + offset), size * sizeof(uint64_t));
  }

  /**
   * @brief writes a int8 buffer to a file.
   * @param file The file to write to.
   * @param buffer The buffer to write.
   */
  inline void write_int8_buffer(const ofstream_ref& file, const int8_buffer& buffer)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_int8_buffer: file is not open");
    file->write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
  }

  /**
   * @brief writes a int8 buffer to a file.
   * @param file The file to write to.
   * @param buffer The buffer to write.
   * @param size The size of the buffer to write.
   * @note The size parameter is in number of elements, not bytes.
   */
  inline void write_int8_buffer(const ofstream_ref& file,
                                const int8_buffer& buffer,
                                size_t size,
                                size_t offset = 0)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_int8_buffer: file is not open");
    MGL_CORE_ASSERT(size <= buffer.size(), "write_bytes: size is greater than buffer size");
    MGL_CORE_ASSERT(offset + size <= buffer.size(),
                    "write_bytes: offset + size is greater than buffer size");
    file->write(reinterpret_cast<const char*>(buffer.data() + offset), size);
  }

  /**
   * @brief writes a int16 buffer to a file.
   * @param file The file to write to.
   * @param buffer The buffer to write.
   */
  inline void write_int16_buffer(const ofstream_ref& file, const int16_buffer& buffer)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_int16_buffer: file is not open");
    file->write(reinterpret_cast<const char*>(buffer.data()), buffer.size() * sizeof(int16_t));
  }

  /**
   * @brief writes a int16 buffer to a file.
   * @param file The file to write to.
   * @param buffer The buffer to write.
   * @param size The size of the buffer to write.
   * @note The size parameter is in number of elements, not bytes.
   */
  inline void write_int16_buffer(const ofstream_ref& file,
                                 const int16_buffer& buffer,
                                 size_t size,
                                 size_t offset = 0)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_int16_buffer: file is not open");
    MGL_CORE_ASSERT(size <= buffer.size(), "write_bytes: size is greater than buffer size");
    MGL_CORE_ASSERT(offset + size <= buffer.size(),
                    "write_bytes: offset + size is greater than buffer size");
    file->write(reinterpret_cast<const char*>(buffer.data() + offset), size * sizeof(int16_t));
  }

  /**
   * @brief writes a int32 buffer to a file.
   * @param file The file to write to.
   * @param buffer The buffer to write.
   */
  inline void write_int32_buffer(const ofstream_ref& file, const int32_buffer& buffer)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_int32_buffer: file is not open");
    file->write(reinterpret_cast<const char*>(buffer.data()), buffer.size() * sizeof(int32_t));
  }

  /**
   * @brief writes a int32 buffer to a file.
   * @param file The file to write to.
   * @param buffer The buffer to write.
   * @param size The size of the buffer to write.
   * @note The size parameter is in number of elements, not bytes.
   */
  inline void write_int32_buffer(const ofstream_ref& file,
                                 const int32_buffer& buffer,
                                 size_t size,
                                 size_t offset = 0)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_int32_buffer: file is not open");
    MGL_CORE_ASSERT(size <= buffer.size(), "write_bytes: size is greater than buffer size");
    MGL_CORE_ASSERT(offset + size <= buffer.size(),
                    "write_bytes: offset + size is greater than buffer size");
    file->write(reinterpret_cast<const char*>(buffer.data() + offset), size * sizeof(int32_t));
  }

  /**
   * @brief writes a int64 buffer to a file.
   * @param file The file to write to.
   * @param buffer The buffer to write.
   */
  inline void write_int64_buffer(const ofstream_ref& file, const int64_buffer& buffer)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_int64_buffer: file is not open");
    file->write(reinterpret_cast<const char*>(buffer.data()), buffer.size() * sizeof(int64_t));
  }

  /**
   * @brief writes a int64 buffer to a file.
   * @param file The file to write to.
   * @param buffer The buffer to write.
   * @param size The size of the buffer to write.
   * @note The size parameter is in number of elements, not bytes.
   */
  inline void write_int64_buffer(const ofstream_ref& file,
                                 const int64_buffer& buffer,
                                 size_t size,
                                 size_t offset = 0)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_int64_buffer: file is not open");
    MGL_CORE_ASSERT(size <= buffer.size(), "write_bytes: size is greater than buffer size");
    MGL_CORE_ASSERT(offset + size <= buffer.size(),
                    "write_bytes: offset + size is greater than buffer size");
    file->write(reinterpret_cast<const char*>(buffer.data() + offset), size * sizeof(int64_t));
  }

  /**
   * @brief writes a float32 buffer to a file.
   * @param file The file to write to.
   * @param buffer The buffer to write.
   */
  inline void write_float32_buffer(const ofstream_ref& file, const float32_buffer& buffer)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_float32_buffer: file is not open");
    file->write(reinterpret_cast<const char*>(buffer.data()), buffer.size() * sizeof(float));
  }

  /**
   * @brief writes a float32 buffer to a file.
   * @param file The file to write to.
   * @param buffer The buffer to write.
   * @param size The size of the buffer to write.
   * @note The size parameter is in number of elements, not bytes.
   */
  inline void write_float32_buffer(const ofstream_ref& file,
                                   const float32_buffer& buffer,
                                   size_t size,
                                   size_t offset = 0)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_float32_buffer: file is not open");
    MGL_CORE_ASSERT(size <= buffer.size(), "write_bytes: size is greater than buffer size");
    MGL_CORE_ASSERT(offset + size <= buffer.size(),
                    "write_bytes: offset + size is greater than buffer size");
    file->write(reinterpret_cast<const char*>(buffer.data() + offset), size * sizeof(float));
  }

  /**
   * @brief writes a float64 buffer to a file.
   * @param file The file to write to.
   * @param buffer The buffer to write.
   */
  inline void write_float64_buffer(const ofstream_ref& file, const float64_buffer& buffer)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_float64_buffer: file is not open");
    file->write(reinterpret_cast<const char*>(buffer.data()), buffer.size() * sizeof(double));
  }

  /**
   * @brief writes a float64 buffer to a file.
   * @param file The file to write to.
   * @param buffer The buffer to write.
   * @param size The size of the buffer to write.
   * @note The size parameter is in number of elements, not bytes.
   */
  inline void write_float64_buffer(const ofstream_ref& file,
                                   const float64_buffer& buffer,
                                   size_t size,
                                   size_t offset = 0)
  {
    MGL_CORE_ASSERT(file->good() && !file->eof(), "write_float64_buffer: file is not open");
    MGL_CORE_ASSERT(size <= buffer.size(), "write_bytes: size is greater than buffer size");
    MGL_CORE_ASSERT(offset + size <= buffer.size(),
                    "write_bytes: offset + size is greater than buffer size");
    file->write(reinterpret_cast<const char*>(buffer.data() + offset), size * sizeof(double));
  }

  /**
   * @brief Opens a file for reading.
   * @param path The path to the file to open.
   * @param mode The mode to open the file in.
   */
  inline ifstream_ref open_read(const path& path, openmode mode = std::ios::binary)
  {
    ifstream_ref result = mgl::create_ref<std::ifstream>(path, std::ios_base::in | mode);
    MGL_CORE_ASSERT(result->is_open(), "open_read: failed to open file {0}", path.string());
    return result;
  }

  /**
   * @brief Opens a file for writing.
   * @param path The path to the file to open.
   * @param mode The mode to open the file in.
   */
  inline ofstream_ref open_write(const path& path, openmode mode = std::ios::binary)
  {
    ofstream_ref result = mgl::create_ref<std::ofstream>(path, std::ios_base::out | mode);
    MGL_CORE_ASSERT(result->is_open(), "open_write: failed to open file {0}", path.string());
    return result;
  }

} // namespace mgl::io
