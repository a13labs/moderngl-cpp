/**
 * @file math.hpp
 * @brief Contains mathematical utility functions and structures.
 */

#pragma once

#include <cstdint>

#define MGL_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MGL_MIN(a, b) (((a) < (b)) ? (a) : (b))

namespace mgl
{
  /**
   * @brief Represents a rectangle.
   */
  struct rect
  {
    int x; /**< The x-coordinate of the top-left corner of the rectangle. */
    int y; /**< The y-coordinate of the top-left corner of the rectangle. */
    int width; /**< The width of the rectangle. */
    int height; /**< The height of the rectangle. */

    /**
     * @brief Constructs a rectangle with the specified position and size.
     * @param x The x-coordinate of the top-left corner.
     * @param y The y-coordinate of the top-left corner.
     * @param w The width of the rectangle.
     * @param h The height of the rectangle.
     */
    rect(int x, int y, int w, int h)
        : x(x)
        , y(y)
        , width(w)
        , height(h)
    { }

    /**
     * @brief Constructs a rectangle with the specified size.
     * The top-left corner is set to (0, 0).
     * @param w The width of the rectangle.
     * @param h The height of the rectangle.
     */
    rect(int w, int h)
        : x(0)
        , y(0)
        , width(w)
        , height(h)
    { }

    /**
     * @brief Default constructor.
     * Constructs a rectangle with all values set to 0.
     */
    rect()
        : x(0)
        , y(0)
        , width(0)
        , height(0)
    { }

    /**
     * @brief Copy constructor.
     * @param other The rectangle to copy from.
     */
    rect(const rect& other)
        : x(other.x)
        , y(other.y)
        , width(other.width)
        , height(other.height)
    { }

    /**
     * @brief Move constructor.
     * @param other The rectangle to move from.
     */
    rect(rect&& other) noexcept
        : x(other.x)
        , y(other.y)
        , width(other.width)
        , height(other.height)
    { }

    /**
     * @brief Destructor.
     */
    ~rect() = default;

    /**
     * @brief Copy assignment operator.
     * @param other The rectangle to copy from.
     * @return A reference to the assigned rectangle.
     */
    rect& operator=(const rect& other)
    {
      x = other.x;
      y = other.y;
      width = other.width;
      height = other.height;
      return *this;
    }
  };

  /**
   * @brief Represents a cube.
   */
  struct cube
  {
    int x; /**< The x-coordinate of the top-left corner of the cube. */
    int y; /**< The y-coordinate of the top-left corner of the cube. */
    int z; /**< The z-coordinate of the top-left corner of the cube. */
    int width; /**< The width of the cube. */
    int height; /**< The height of the cube. */
    int depth; /**< The depth of the cube. */

    /**
     * @brief Constructs a cube with the specified position and size.
     * @param x The x-coordinate of the top-left corner.
     * @param y The y-coordinate of the top-left corner.
     * @param z The z-coordinate of the top-left corner.
     * @param w The width of the cube.
     * @param h The height of the cube.
     * @param d The depth of the cube.
     */
    cube(int x, int y, int z, int w, int h, int d)
        : x(x)
        , y(y)
        , z(z)
        , width(w)
        , height(h)
        , depth(d)
    { }

    /**
     * @brief Constructs a cube with the specified size.
     * The top-left corner is set to (0, 0, 0).
     * @param w The width of the cube.
     * @param h The height of the cube.
     * @param d The depth of the cube.
     */
    cube(int w, int h, int d)
        : x(0)
        , y(0)
        , z(0)
        , width(w)
        , height(h)
        , depth(d)
    { }

    /**
     * @brief Default constructor.
     * Constructs a cube with all values set to 0.
     */
    cube()
        : x(0)
        , y(0)
        , z(0)
        , width(0)
        , height(0)
        , depth(0)
    { }

    /**
     * @brief Copy constructor.
     * @param other The cube to copy from.
     */
    cube(const cube& other)
        : x(other.x)
        , y(other.y)
        , z(other.z)
        , width(other.width)
        , height(other.height)
        , depth(other.depth)
    { }

    /**
     * @brief Destructor.
     */
    ~cube() = default;

    /**
     * @brief Copy assignment operator.
     * @param other The cube to copy from.
     * @return A reference to the assigned cube.
     */
    cube& operator=(const cube& other)
    {
      x = other.x;
      y = other.y;
      z = other.z;
      width = other.width;
      height = other.height;
      depth = other.depth;
      return *this;
    }
  };

  /**
   * @brief Represents a size.
   */
  struct size
  {
    int width; /**< The width of the size. */
    int height; /**< The height of the size. */

    /**
     * @brief Constructs a size with the specified width and height.
     * @param w The width of the size.
     * @param h The height of the size.
     */
    size(int w, int h)
        : width(w)
        , height(h)
    { }

    /**
     * @brief Default constructor.
     * Constructs a size with all values set to 0.
     */
    size()
        : width(0)
        , height(0)
    { }

    /**
     * @brief Copy constructor.
     * @param other The size to copy from.
     */
    size(const size& other)
        : width(other.width)
        , height(other.height)
    { }

    /**
     * @brief Destructor.
     */
    ~size() = default;

    /**
     * @brief Copy assignment operator.
     * @param other The size to copy from.
     * @return A reference to the assigned size.
     */
    size& operator=(const size& other)
    {
      width = other.width;
      height = other.height;
      return *this;
    }
  };

  /**
   * @brief The null 2D viewport.
   * Represents a rectangle with all values set to 0.
   */
  extern const rect null_viewport_2d;

  /**
   * @brief The null 3D viewport.
   * Represents a cube with all values set to 0.
   */
  extern const cube null_viewport_3d;

  /**
   * @brief The null size.
   * Represents a size with all values set to 0.
   */
  extern const size null_size;

  /**
   * @brief Equality comparison operator for rectangles.
   * @param lhs The left-hand side rectangle.
   * @param rhs The right-hand side rectangle.
   * @return True if the rectangles are equal, false otherwise.
   */
  inline bool operator==(const rect& lhs, const rect& rhs)
  {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.width == rhs.width && lhs.height == rhs.height;
  }

  /**
   * @brief Equality comparison operator for cubes.
   * @param lhs The left-hand side cube.
   * @param rhs The right-hand side cube.
   * @return True if the cubes are equal, false otherwise.
   */
  inline bool operator==(const cube& lhs, const cube& rhs)
  {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.width == rhs.width &&
           lhs.height == rhs.height && lhs.depth == rhs.depth;
  }

  /**
   * @brief Equality comparison operator for sizes.
   * @param lhs The left-hand side size.
   * @param rhs The right-hand side size.
   * @return True if the sizes are equal, false otherwise.
   */
  inline bool operator==(const size& lhs, const size& rhs)
  {
    return lhs.width == rhs.width && lhs.height == rhs.height;
  }

  /**
   * @brief Inequality comparison operator for rectangles.
   * @param lhs The left-hand side rectangle.
   * @param rhs The right-hand side rectangle.
   * @return True if the rectangles are not equal, false otherwise.
   */
  inline bool operator!=(const rect& lhs, const rect& rhs)
  {
    return !(lhs == rhs);
  }

  /**
   * @brief Inequality comparison operator for cubes.
   * @param lhs The left-hand side cube.
   * @param rhs The right-hand side cube.
   * @return True if the cubes are not equal, false otherwise.
   */
  inline bool operator!=(const cube& lhs, const cube& rhs)
  {
    return !(lhs == rhs);
  }

  /**
   * @brief Inequality comparison operator for sizes.
   * @param lhs The left-hand side size.
   * @param rhs The right-hand side size.
   * @return True if the sizes are not equal, false otherwise.
   */
  inline bool operator!=(const size& lhs, const size& rhs)
  {
    return !(lhs == rhs);
  }
} // namespace mgl

namespace mgl::math
{
  /**
   * @brief The mathematical constant pi.
   */
  static constexpr double pi = 3.14159265358979323846;

  /**
   * @brief Converts degrees to radians.
   * @param degrees The angle in degrees.
   * @return The angle in radians.
   */
  inline double deg2rad(double degrees)
  {
    return degrees * pi / 180.0;
  }

  /**
   * @brief Converts radians to degrees.
   * @param radians The angle in radians.
   * @return The angle in degrees.
   */
  inline double rad2deg(double radians)
  {
    return radians * 180.0 / pi;
  }

  /**
   * @brief Returns the next power of two of the specified value.
   * @param value The value.
   * @return The next power of two of the specified value.
   */
  inline uint8_t next_power_of_two(uint8_t value)
  {
    --value;
    value |= value >> 1;
    value |= value >> 2;
    ++value;
    return value;
  }

  /**
   * @brief Returns the next power of two of the specified value.
   * @param value The value.
   * @return The next power of two of the specified value.
   */
  inline uint16_t next_power_of_two(uint16_t value)
  {
    --value;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    ++value;
    return value;
  }

  /**
   * @brief Returns the next power of two of the specified value.
   * @param value The value.
   * @return The next power of two of the specified value.
   */
  inline uint32_t next_power_of_two(uint32_t value)
  {
    --value;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    ++value;
    return value;
  }

  /**
   * @brief Returns the next power of two of the specified value.
   * @param value The value.
   * @return The next power of two of the specified value.
   */
  inline uint64_t next_power_of_two(uint64_t value)
  {
    --value;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value |= value >> 32;
    ++value;
    return value;
  }

  /**
   * @brief Returns the next power of two of the specified value.
   * @param value The value.
   * @return The next power of two of the specified value.
   */
  inline int8_t next_power_of_two(int8_t value)
  {
    return (int8_t)next_power_of_two((uint8_t)value);
  }

  /**
   * @brief Returns the next power of two of the specified value.
   * @param value The value.
   * @return The next power of two of the specified value.
   */
  inline int16_t next_power_of_two(int16_t value)
  {
    return (int16_t)next_power_of_two((uint16_t)value);
  }

  /**
   * @brief Returns the next power of two of the specified value.
   * @param value The value.
   * @return The next power of two of the specified value.
   */
  inline int32_t next_power_of_two(int32_t value)
  {
    return (int32_t)next_power_of_two((uint32_t)value);
  }

  /**
   * @brief Returns the next power of two of the specified value.
   * @param value The value.
   * @return The next power of two of the specified value.
   */
  inline int64_t next_power_of_two(int64_t value)
  {
    return (int64_t)next_power_of_two((uint64_t)value);
  }

  /**
   * @brief Returns the floor of the specified value.
   * @param f The value.
   * @return The floor of the specified value.
   */
  static inline float floor(float f)
  {
    return (float)((f >= 0 || (float)(int)f == f) ? (int)f : (int)f - 1);
  }

  /**
   * @brief Returns the ceiling of the specified value.
   * @param f The value.
   * @return The ceiling of the specified value.
   */
  static inline float ceil(float f)
  {
    return (float)((f >= 0) ? ((int)f == f ? (int)f : (int)f + 1) : (int)f);
  }

  /**
   * @brief Returns the absolute value of the specified value.
   * @param f The value.
   * @return The absolute value of the specified value.
   */
  static inline float abs(float f)
  {
    return (f >= 0) ? f : -f;
  }

} // namespace mgl::math
