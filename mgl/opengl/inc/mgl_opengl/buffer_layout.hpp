#pragma once

#include <string>

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
    The buffer info contains the size of the buffer, the number of nodes in the buffer, and the divisor.
    */
    struct info
    {
      int size;
      int nodes;
      int divisor;

      info()
          : size(0)
          , nodes(0)
          , divisor(0)
      { }

      info(int size, int nodes, int divisor)
          : size(size)
          , nodes(nodes)
          , divisor(divisor)
      { }

      info(const info& other)
          : size(other.size)
          , nodes(other.nodes)
          , divisor(other.divisor)
      { }

      bool is_invalid() { return size == 0 && nodes == 0 && divisor == 0; }

      info& operator=(const info& other)
      {
        size = other.size;
        nodes = other.nodes;
        divisor = other.divisor;
        return *this;
      }

      bool operator==(const info& other)
      {
        return size == other.size && nodes == other.nodes && divisor == other.divisor;
      }

      bool operator!=(const info& other) { return !(*this == other); }

      static info invalid()
      {
        info invalid;
        invalid.size = 0;
        invalid.nodes = 0;
        invalid.divisor = 0;
        return invalid;
      }
    };

private:
    /*
    @brief
    An element is a single node in the buffer layout.
    */
    struct element
    {
      int size;
      int count;
      int type;
      bool normalize;

      element()
          : size(0)
          , count(0)
          , type(0)
          , normalize(false)
      { }

      element(int size, int count, int type, bool normalize)
          : size(size)
          , count(count)
          , type(type)
          , normalize(normalize)
      { }

      element(const element& other)
          : size(other.size)
          , count(other.count)
          , type(other.type)
          , normalize(other.normalize)
      { }

      const bool is_invalid() const
      {
        return size == 0 && count == 0 && type == 0 && normalize == false;
      }

      element& operator=(const element& other)
      {
        size = other.size;
        count = other.count;
        type = other.type;
        normalize = other.normalize;
        return *this;
      }

      bool operator==(const element& other)
      {
        return size == other.size && count == other.count && type == other.type &&
               normalize == other.normalize;
      }

      bool operator!=(const element& other) { return !(*this == other); }
    };

    /*
    @brief
    An iterator for the buffer layout. Helps parse the layout string.
    */
    struct iterator
    {
      iterator(const char* str);

      iterator& operator++();

      const element& value() const { return m_element; }

      iterator& operator=(const iterator& other)
      {
        m_ptr = other.m_ptr;
        m_element = other.m_element;
        return *this;
      }

      bool operator==(const iterator& other) { return m_ptr == other.m_ptr; }

      bool operator!=(const iterator& other) { return !(*this == other); }

  private:
      const char* m_ptr;
      element m_element;
    };

public:
    buffer_layout(const std::string& layout);

    iterator begin() { return iterator(m_layout.c_str()); }
    iterator end() { return iterator(nullptr); }
    buffer_layout::info layout_info() const { return m_layout_info; }

private:
    std::string m_layout;
    buffer_layout::info m_layout_info;
  };

} // namespace  mgl::opengl