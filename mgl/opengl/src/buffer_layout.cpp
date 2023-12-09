#include "mgl_core/string.hpp"

#include "mgl_opengl/buffer_layout.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  // A buffer layout is a short string describing the layout of data in a vertex buffer object (VBO).
  // The buffer layout looks like: [count]type[size] [[count]type[size]...] [/usage]
  void buffer_layout::parse_layout(const std::string& layout)
  {
    m_layout = layout;

    // The format is used to describe the layout of the data in the VBO
    for(buffer_layout::iterator it = begin(); it != end(); ++it)
    {
      if(it.value().is_invalid())
      {
        m_layout = "";
        m_elements.clear();
        m_stride = 0;
        m_divisor = 0;
        return;
      }

      m_stride += it.value().size;

      if(it.value().type != 0)
      {
        m_elements.push_back(it.value());
      }
    }

    // The divisor is used to describe the usage of the data in the VBO
    mgl::string_list split = mgl::split(layout, '/');

    if(split.size() > 1)
    {
      std::string divisor = split[1];
      if(divisor == "i")
      {
        // /i per instance. Successive values from the buffer are passed to each instance.
        m_divisor = 1;
      }
      else if(divisor == "r")
      {
        // /r per render. . the first buffer value is passed to every vertex of every instance. ie. behaves like a uniform.
        m_divisor = 0x7fffffff;
      }
      else if(divisor == "v")
      {
        // /v per vertex. Successive values from the buffer are passed to each vertex. This is the default behavior if usage is omitted.
        m_divisor = 0;
      }
      else
      {
        // Invalid divisor
        m_layout = "";
        m_elements.clear();
        m_stride = 0;
        m_divisor = 0;
      }
    }
    else
    {
      m_divisor = 0;
    }
  }

  // The iterator is used to parse the layout string and return the next element in the layout
  buffer_layout::iterator::iterator(const char* ptr, int offset)
      : m_ptr(ptr)
      , m_offset(offset)
  {
    if(ptr == nullptr)
    {
      m_element = { 0, 0, 0, 0, false };
      return;
    }

    m_element.count = 0;
    while(true)
    {
      char chr = *m_ptr++;

      // Skip spaces
      if(isspace(chr))
      {
        continue;
      }

      // While it's a digit, accumulate it
      if(isdigit(chr))
      {
        m_element.count = m_element.count * 10 + (chr - '0');
        continue;
      }

      // If it's the end of the string or a divisor the element is invalid, return
      if(chr == 0 || chr == '/')
      {
        --m_ptr;
        m_element = { 0, 0, 0, 0, false };
        return;
      }

      // it's a data type, if there's no count, it's 1 by default
      m_element.count = m_element.count ? m_element.count : 1;

      char data_type = chr;
      int size = -1;

      // Now we need to check if there's a size
      chr = *m_ptr++;

      switch(chr)
      {
        // If it's a digit, accumulate it
        case '0' ... '9':
          size = chr - '0';
          // It is mandoatory that the if there is a next character, it's a divisor or a space
          if(*m_ptr && *m_ptr != ' ' && *m_ptr != '/')
          {
            m_element = { 0, 0, 0, 0, false };
            return;
          }
          break;
        // If it's a space, it's the end of the element
        // If it's the end of the string or a divisor the size it's the default for the data type
        case 0:
        case '/': --m_ptr; break;
        case ' ': break;
        // Otherwise it's invalid
        default: m_element = { 0, 0, 0, 0, false }; return;
      }

      // If the size is 1, it's a normalized format
      m_element.normalize = size == 1;
      m_element.offset = m_offset;

      switch(data_type)
      {
        // float
        case 'f':
          // If the size is -1, it's the default for the data type
          size = size == -1 ? 4 : size;
          switch(size)
          {
            case 1: m_element.type = GL_UNSIGNED_BYTE; break;
            case 2: m_element.type = GL_HALF_FLOAT; break;
            case 4: m_element.type = GL_FLOAT; break;
            case 8: m_element.type = GL_DOUBLE; break;
            default: m_element = { 0, 0, 0, 0, false }; return;
          }
          m_element.size = size * m_element.count;
          m_offset += m_element.size;
          return;
        // signed integer
        case 'i':
          // If the size is -1, it's the default for the data type
          size = size == -1 ? 4 : size;
          switch(size)
          {
            case 1: m_element.type = GL_BYTE; break;
            case 2: m_element.type = GL_SHORT; break;
            case 4: m_element.type = GL_INT; break;
            case 8: m_element.type = GL_DOUBLE; break;
            default: m_element = { 0, 0, 0, 0, false }; return;
          }
          m_element.size = size * m_element.count;
          m_offset += m_element.size;
          return;
        // unsigned int
        case 'u':
          size = size == -1 ? 4 : size;
          switch(size)
          {
            case 1: m_element.type = GL_UNSIGNED_BYTE; break;
            case 2: m_element.type = GL_UNSIGNED_SHORT; break;
            case 4: m_element.type = GL_UNSIGNED_INT; break;
            default: m_element = { 0, 0, 0, 0, false }; return;
          }
          m_element.size = size * m_element.count;
          return;
        // padding
        case 'x':
          size = size == -1 ? 1 : size;
          m_element.size = size * m_element.count;
          m_element.type = 0;
          m_offset += m_element.size;
          return;
        default: m_element = { 0, 0, 0, 0, false }; return;
      }
    }
  }

  // Returns the next element in the layout
  buffer_layout::iterator& buffer_layout::iterator::operator++()
  {
    if(*m_ptr == 0 || *m_ptr == '/')
    {
      m_ptr = nullptr;
      m_element = { 0, 0, 0, 0, false };
      return *this;
    }

    *this = iterator(m_ptr, m_offset);
    return *this;
  }

} // namespace mgl::opengl