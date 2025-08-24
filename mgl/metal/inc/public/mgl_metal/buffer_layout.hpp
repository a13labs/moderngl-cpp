/**
 * @file buffer_layout.hpp
 * @brief Defines the mgl::metal::buffer_layout class for describing the layout of data in a vertex buffer object (VBO).
 *
 * A buffer layout is a short string describing the layout of data in a VBO, which typically contains a homogeneous array of C-like structures.
 * The buffer layout describes the structure of each element in the array, such as the number of components, their types, sizes, and normalization.
 * For example, a layout string "2f8" describes two double-precision floats per element.
 *
 * Classes:
 * - buffer_layout: Parses and stores the buffer layout string, provides access to element descriptions, stride, and divisor.
 *   - element: Represents a single attribute in the buffer layout, including size, count, offset, type, and normalization.
 *   - iterator: Helper for parsing the layout string into elements.
 *
 * Members:
 * - m_layout: The layout string describing the buffer structure.
 * - m_elements: List of parsed elements describing each attribute.
 * - m_stride: The total stride (in bytes) of each element in the buffer.
 * - m_divisor: The divisor for instanced rendering.
 *
 * Usage:
 * - Construct buffer_layout with a layout string to parse and describe buffer attributes.
 * - Access individual elements, stride, and divisor for buffer configuration.
 */
#pragma once

#include "mgl_core/containers.hpp"

namespace mgl::metal
{
  class buffer_layout
  {
public:
    struct element
    {
      int size;
      int count;
      int offset;
      int type;
      bool normalize;

      element()
          : size(0)
          , count(0)
          , offset(0)
          , type(0)
          , normalize(false)
      { }

      element(int size, int count, int offset, int type, bool normalize)
          : size(size)
          , count(count)
          , offset(offset)
          , type(type)
          , normalize(normalize)
      { }

      element(const element& other)
          : size(other.size)
          , count(other.count)
          , offset(other.offset)
          , type(other.type)
          , normalize(other.normalize)
      { }

      const bool is_invalid() const
      {
        return size == 0 && count == 0 && offset == 0 && type == 0 && normalize == false;
      }

      element& operator=(const element& other)
      {
        size = other.size;
        count = other.count;
        offset = other.offset;
        type = other.type;
        normalize = other.normalize;
        return *this;
      }

      bool operator==(const element& other)
      {
        return size == other.size && count == other.count && offset == other.offset &&
               type == other.type && normalize == other.normalize;
      }
      bool operator!=(const element& other) { return !(*this == other); }
    };

public:
    buffer_layout(const std::string& layout)
        : m_layout()
        , m_elements()
        , m_stride(0)
        , m_divisor(0)
    {
      parse_layout(layout);
    }

    buffer_layout(const buffer_layout& other)
        : m_layout(other.m_layout)
        , m_elements(other.m_elements)
        , m_stride(other.m_stride)
        , m_divisor(other.m_divisor)
    { }

    buffer_layout(buffer_layout&& other)
        : m_layout(std::move(other.m_layout))
        , m_elements(std::move(other.m_elements))
        , m_stride(other.m_stride)
        , m_divisor(other.m_divisor)
    { }

    const std::string& layout() const { return m_layout; }

    const mgl::list<element>& elements() const { return m_elements; }

    int stride() const { return m_stride; }

    int divisor() const { return m_divisor; }

    size_t size() const { return m_elements.size(); }

    bool is_invalid() const { return m_elements.empty(); }

    bool operator==(const buffer_layout& other) { return m_layout == other.m_layout; }

    bool operator!=(const buffer_layout& other) { return !(*this == other); }

    buffer_layout& operator=(const buffer_layout& other)
    {
      m_layout = other.m_layout;
      m_elements = other.m_elements;
      m_stride = other.m_stride;
      m_divisor = other.m_divisor;
      return *this;
    }

    buffer_layout& operator=(buffer_layout&& other)
    {
      m_layout = std::move(other.m_layout);
      m_elements = std::move(other.m_elements);
      m_stride = other.m_stride;
      m_divisor = other.m_divisor;
      return *this;
    }

    buffer_layout& operator=(const std::string& layout)
    {
      parse_layout(layout);
      return *this;
    }

    const element& operator[](size_t index) const { return m_elements[index]; }

    bool operator==(const std::string& layout) { return m_layout == layout; }

    bool operator!=(const std::string& layout) { return !(*this == layout); }

    bool operator==(const buffer_layout& other) const { return m_layout == other.m_layout; }

    bool operator!=(const buffer_layout& other) const { return !(*this == other); }

private:
    struct iterator
    {
      iterator(const char* str, int offset = 0);

      iterator& operator++();

      const element& value() const { return m_element; }

      iterator& operator=(const iterator& other)
      {
        m_ptr = other.m_ptr;
        m_element = other.m_element;
        m_offset = other.m_offset;
        return *this;
      }

      bool operator==(const iterator& other) { return m_ptr == other.m_ptr; }

      bool operator!=(const iterator& other) { return !(*this == other); }

  private:
      const char* m_ptr;
      element m_element;
      int m_offset;
    };

    iterator begin() { return iterator(m_layout.c_str()); }

    iterator end() { return iterator(nullptr); }

    void parse_layout(const std::string& layout);

private:
    std::string m_layout;
    mgl::list<element> m_elements;
    int m_stride;
    int m_divisor;
  };

} // namespace  mgl::metal