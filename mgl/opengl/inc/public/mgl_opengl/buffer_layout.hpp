/**
 * @file buffer_layout.hpp
 * @brief Defines the mgl::opengl::buffer_layout class for describing the layout of data in a vertex buffer object (VBO).
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

#include "mgl_core/string.hpp"

/**
 * @class buffer_layout
 * @brief Describes the layout of data in a vertex buffer object (VBO).
 *
 * A buffer layout is a short string describing the structure of elements in a VBO,
 * which typically contains a homogeneous array of C-like structures. The layout string
 * specifies the type, size, and arrangement of each element in the buffer.
 * For example, "2f8" describes an array of 2 double-precision floats per element.
 *
 * @section Example
 * A buffer containing high-precision 2D vertex positions might use the layout "2f8",
 * meaning each element consists of two floats, each 8 bytes wide (double).
 *
 * @section Members
 * - element: Represents a single node in the buffer layout, describing its size, count, offset, type, and normalization.
 * - m_layout: The layout string.
 * - m_elements: List of parsed elements from the layout string.
 * - m_stride: The stride (total size in bytes) of each element.
 * - m_divisor: The divisor for instanced rendering.
 *
 * @section Methods
 * - buffer_layout(const std::string& layout): Constructs and parses the layout string.
 * - layout(): Returns the layout string.
 * - elements(): Returns the list of elements.
 * - stride(): Returns the stride of each element.
 * - divisor(): Returns the divisor value.
 * - size(): Returns the number of elements.
 * - is_invalid(): Checks if the layout is invalid.
 * - operator[]: Accesses an element by index.
 * - operator==, operator!=: Compares layouts or elements.
 * - operator=(...): Assignment operators for layout and elements.
 *
 * @section Internal
 * - iterator: Helper struct for parsing the layout string.
 * - parse_layout(): Parses the layout string into elements.
 */
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

      /// @brief Default constructor initializes an element with zero size, count, offset, type, and normalization set to false.
      element()
          : size(0)
          , count(0)
          , offset(0)
          , type(0)
          , normalize(false)
      { }

      /// @brief Parameterized constructor initializes an element with the given attributes.
      /// @param size The size of the element in bytes.
      /// @param count The number of components in the element.
      /// @param offset The offset of the element within the buffer.
      /// @param type The data type of the element.
      /// @param normalize Whether the element should be normalized.
      element(int size, int count, int offset, int type, bool normalize)
          : size(size)
          , count(count)
          , offset(offset)
          , type(type)
          , normalize(normalize)
      { }

      /// @brief Copy constructor initializes an element with the attributes of another element.
      /// @param other The element to copy.
      element(const element& other)
          : size(other.size)
          , count(other.count)
          , offset(other.offset)
          , type(other.type)
          , normalize(other.normalize)
      { }

      /// @brief Checks if the element is invalid (all attributes are zero or false).
      /// @return True if the element is invalid, false otherwise.
      const bool is_invalid() const
      {
        return size == 0 && count == 0 && offset == 0 && type == 0 && normalize == false;
      }

      /// @brief Assignment operator.
      /// @param other The element to copy.
      /// @return A reference to this element.
      element& operator=(const element& other)
      {
        size = other.size;
        count = other.count;
        offset = other.offset;
        type = other.type;
        normalize = other.normalize;
        return *this;
      }

      /// @brief Equality operator.
      /// @param other The element to compare.
      /// @return True if the elements are equal, false otherwise.
      bool operator==(const element& other)
      {
        return size == other.size && count == other.count && offset == other.offset &&
               type == other.type && normalize == other.normalize;
      }

      /// @brief Inequality operator.
      /// @param other The element to compare.
      /// @return True if the elements are not equal, false otherwise.
      bool operator!=(const element& other) { return !(*this == other); }
    };

public:
    /// @brief Constructs a buffer layout from a layout string.
    /// @param layout The layout string to parse.
    buffer_layout(const std::string& layout)
        : m_layout()
        , m_elements()
        , m_stride(0)
        , m_divisor(0)
    {
      parse_layout(layout);
    }

    /// @brief Copy constructor initializes a buffer layout with the attributes of another layout.
    /// @param other The layout to copy.
    buffer_layout(const buffer_layout& other)
        : m_layout(other.m_layout)
        , m_elements(other.m_elements)
        , m_stride(other.m_stride)
        , m_divisor(other.m_divisor)
    { }

    /// @brief Move constructor initializes a buffer layout by transferring the attributes of another layout.
    /// @param other The layout to move from.
    buffer_layout(buffer_layout&& other)
        : m_layout(std::move(other.m_layout))
        , m_elements(std::move(other.m_elements))
        , m_stride(other.m_stride)
        , m_divisor(other.m_divisor)
    { }

    /// @brief Returns the layout string.
    /// @return The layout string.
    const std::string& layout() const { return m_layout; }

    /// @brief Returns the list of elements in the buffer layout.
    /// @return The list of elements in the buffer layout.
    const mgl::list<element>& elements() const { return m_elements; }

    /// @brief Returns the stride of the buffer layout.
    /// @return The stride of the buffer layout.
    int stride() const { return m_stride; }

    /// @brief Returns the divisor of the buffer layout.
    /// @return The divisor of the buffer layout.
    int divisor() const { return m_divisor; }

    /// @brief Returns the size of the buffer layout.
    /// @return The size of the buffer layout.
    size_t size() const { return m_elements.size(); }

    /// @brief Checks if the buffer layout is invalid (no elements).
    /// @return True if the buffer layout is invalid, false otherwise.
    bool is_invalid() const { return m_elements.empty(); }

    /// @brief Equality operator.
    /// @param other The layout to compare.
    /// @return True if the layouts are equal, false otherwise.
    bool operator==(const buffer_layout& other) { return m_layout == other.m_layout; }

    /// @brief Inequality operator.
    /// @param other The layout to compare.
    /// @return True if the layouts are not equal, false otherwise.
    bool operator!=(const buffer_layout& other) { return !(*this == other); }

    /// @brief Assignment operator.
    /// @param other The layout to assign from.
    /// @return A reference to this layout.
    buffer_layout& operator=(const buffer_layout& other)
    {
      m_layout = other.m_layout;
      m_elements = other.m_elements;
      m_stride = other.m_stride;
      m_divisor = other.m_divisor;
      return *this;
    }

    /// @brief Move assignment operator.
    /// @param other The layout to move from.
    /// @return A reference to this layout.
    buffer_layout& operator=(buffer_layout&& other)
    {
      m_layout = std::move(other.m_layout);
      m_elements = std::move(other.m_elements);
      m_stride = other.m_stride;
      m_divisor = other.m_divisor;
      return *this;
    }

    /// @brief Assignment operator.
    /// @param layout The layout string to assign from.
    /// @return A reference to this layout.
    buffer_layout& operator=(const std::string& layout)
    {
      parse_layout(layout);
      return *this;
    }

    /// @brief Subscript operator.
    /// @param index The index of the element to access.
    /// @return A const reference to the element at the specified index.
    const element& operator[](size_t index) const { return m_elements[index]; }

    /// @brief Equality operator.
    /// @param layout The layout string to compare.
    /// @return True if the layouts are equal, false otherwise.
    bool operator==(const std::string& layout) { return m_layout == layout; }

    /// @brief Inequality operator.
    /// @param layout The layout string to compare.
    /// @return True if the layouts are not equal, false otherwise.
    bool operator!=(const std::string& layout) { return !(*this == layout); }

    /// @brief Equality operator.
    /// @param other The layout to compare.
    /// @return True if the layouts are equal, false otherwise.
    bool operator==(const buffer_layout& other) const { return m_layout == other.m_layout; }

    /// @brief Inequality operator.
    /// @param other The layout to compare.
    /// @return True if the layouts are not equal, false otherwise.
    bool operator!=(const buffer_layout& other) const { return !(*this == other); }

private:
    /*
    @brief
    An iterator for the buffer layout. Helps parse the layout string.
    */
    struct iterator
    {
      /// @brief Constructs an iterator for the buffer layout.
      /// @param str The layout string to parse.
      /// @param offset The initial offset within the string.
      iterator(const char* str, int offset = 0);

      /// @brief Prefix increment operator.
      /// @return A reference to this iterator.
      iterator& operator++();

      /// @brief Returns the current element.
      /// @return A const reference to the current element.
      const element& value() const { return m_element; }

      /// @brief Assignment operator.
      /// @param other The iterator to assign from.
      /// @return A reference to this iterator.
      iterator& operator=(const iterator& other)
      {
        m_ptr = other.m_ptr;
        m_element = other.m_element;
        m_offset = other.m_offset;
        return *this;
      }

      /// @brief Equality operator.
      /// @param other The iterator to compare with.
      /// @return True if the iterators are equal, false otherwise.
      bool operator==(const iterator& other) { return m_ptr == other.m_ptr; }

      /// @brief Inequality operator.
      /// @param other The iterator to compare with.
      /// @return True if the iterators are not equal, false otherwise.
      bool operator!=(const iterator& other) { return !(*this == other); }

  private:
      const char* m_ptr;
      element m_element;
      int m_offset;
    };

    /// @brief Returns an iterator to the beginning of the layout.
    /// @return An iterator to the beginning of the layout.
    iterator begin() { return iterator(m_layout.c_str()); }

    /// @brief Returns an iterator to the end of the layout.
    /// @return An iterator to the end of the layout.
    iterator end() { return iterator(nullptr); }

    /// @brief Parses the layout string.
    /// @param layout The layout string to parse.
    void parse_layout(const std::string& layout);

private:
    std::string m_layout;
    mgl::list<element> m_elements;
    int m_stride;
    int m_divisor;
  };

} // namespace  mgl::opengl