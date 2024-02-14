#pragma once

#include "mgl_core/string.hpp"

namespace mgl::opengl
{
  /*
  @brief
  A buffer layout is a short string describing the layout of data in a vertex buffer object (VBO).
  A VBO often contains a homogeneous array of C-like structures. 
  The buffer info describes what each element of the array looks like. 
  For example, a buffer containing an array of high-precision 2D vertex positions might have the info "2f8" - each element of the array consists of two floats, each float being 8 bytes wide, ie. a double.
  */
  class buffer_layout
  {
public:
    /*
    @brief
    An element is a single node in the buffer layout.
    */
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

private:
    /*
    @brief
    An iterator for the buffer layout. Helps parse the layout string.
    */
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

} // namespace  mgl::opengl