#include "mgl_metal/buffer_layout.hpp"
#include "mgl_core/string.hpp"

namespace mgl::metal
{
  // Reuse the parsing logic from the OpenGL implementation as a placeholder.
  // The format is identical for the buffer layout string.
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
      if(isspace(chr))
      {
        continue;
      }
      if(isdigit(chr))
      {
        m_element.count = m_element.count * 10 + (chr - '0');
        continue;
      }
      if(chr == 0 || chr == '/')
      {
        --m_ptr;
        m_element = { 0, 0, 0, 0, false };
        return;
      }

      m_element.count = m_element.count ? m_element.count : 1;
      char data_type = chr;
      int size = -1;
      chr = *m_ptr++;

      switch(chr)
      {
        case '0' ... '9': size = chr - '0'; if(*m_ptr && *m_ptr != ' ' && *m_ptr != '/') { m_element = {0,0,0,0,false}; return; } break;
        case 0:
        case '/': --m_ptr; break;
        case ' ': break;
        default: m_element = { 0, 0, 0, 0, false }; return;
      }

      m_element.normalize = size == 1;
      m_element.offset = m_offset;

      switch(data_type)
      {
        case 'f':
          size = size == -1 ? 4 : size;
          switch(size)
          {
            case 1: m_element.type = 5121; break;
            case 2: m_element.type = 5131; break;
            case 4: m_element.type = 5126; break;
            case 8: m_element.type = 5132; break;
            default: m_element = {0,0,0,0,false}; return;
          }
          m_element.size = size * m_element.count;
          m_offset += m_element.size;
          return;
        case 'i':
          size = size == -1 ? 4 : size;
          switch(size)
          {
            case 1: m_element.type = 5120; break;
            case 2: m_element.type = 5122; break;
            case 4: m_element.type = 5124; break;
            case 8: m_element.type = 5132; break;
            default: m_element = {0,0,0,0,false}; return;
          }
          m_element.size = size * m_element.count;
          m_offset += m_element.size;
          return;
        case 'u':
          size = size == -1 ? 4 : size;
          switch(size)
          {
            case 1: m_element.type = 5121; break;
            case 2: m_element.type = 5123; break;
            case 4: m_element.type = 5125; break;
            default: m_element = {0,0,0,0,false}; return;
          }
          m_element.size = size * m_element.count;
          return;
        case 'x':
          size = size == -1 ? 1 : size;
          m_element.size = size * m_element.count;
          m_element.type = 0;
          m_offset += m_element.size;
          return;
        default: m_element = {0,0,0,0,false}; return;
      }
    }
  }

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

  void buffer_layout::parse_layout(const std::string& layout)
  {
    m_layout = layout;
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

  mgl::string_list split = mgl::split(layout, '/');
    if(split.size() > 1)
    {
      std::string divisor = split[1];
      if(divisor == "i") m_divisor = 1;
      else if(divisor == "r") m_divisor = 0x7fffffff;
      else if(divisor == "v") m_divisor = 0;
      else { m_layout = ""; m_elements.clear(); m_stride = 0; m_divisor = 0; }
    }
    else { m_divisor = 0; }
  }

} // namespace mgl::metal
