#pragma once

#define MGL_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MGL_MIN(a, b) (((a) < (b)) ? (a) : (b))
namespace mgl::core
{
  static constexpr double pi = 3.14159265358979323846;

  inline double deg2rad(double degrees)
  {
    return degrees * pi / 180.0;
  }

  struct rect
  {
    int x;
    int y;
    int width;
    int height;

    rect(int x, int y, int w, int h)
        : x(x)
        , y(y)
        , width(w)
        , height(h)
    { }

    rect(int w, int h)
        : x(0)
        , y(0)
        , width(w)
        , height(h)
    { }

    rect()
        : x(0)
        , y(0)
        , width(0)
        , height(0)
    { }

    rect(const rect& other)
        : x(other.x)
        , y(other.y)
        , width(other.width)
        , height(other.height)
    { }

    rect(rect&& other) noexcept
        : x(other.x)
        , y(other.y)
        , width(other.width)
        , height(other.height)
    { }

    ~rect() = default;

    rect& operator=(const rect& other)
    {
      x = other.x;
      y = other.y;
      width = other.width;
      height = other.height;
      return *this;
    }
  };

  struct cube
  {
    int x;
    int y;
    int z;
    int width;
    int height;
    int depth;

    cube(int x, int y, int z, int w, int h, int d)
        : x(x)
        , y(y)
        , z(z)
        , width(w)
        , height(h)
        , depth(d)
    { }

    cube(int w, int h, int d)
        : x(0)
        , y(0)
        , z(0)
        , width(w)
        , height(h)
        , depth(d)
    { }

    cube()
        : x(0)
        , y(0)
        , z(0)
        , width(0)
        , height(0)
        , depth(0)
    { }

    cube(const cube& other)
        : x(other.x)
        , y(other.y)
        , z(other.z)
        , width(other.width)
        , height(other.height)
        , depth(other.depth)
    { }

    ~cube() = default;

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

  struct size
  {
    int width;
    int height;

    size(int w, int h)
        : width(w)
        , height(h)
    { }

    size()
        : width(0)
        , height(0)
    { }

    size(const size& other)
        : width(other.width)
        , height(other.height)
    { }

    ~size() = default;

    size& operator=(const size& other)
    {
      width = other.width;
      height = other.height;
      return *this;
    }
  };

  extern const rect null_viewport_2d;
  extern const cube null_viewport_3d;
  extern const size null_size;

  inline bool operator==(const rect& lhs, const rect& rhs)
  {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.width == rhs.width && lhs.height == rhs.height;
  }

  inline bool operator==(const cube& lhs, const cube& rhs)
  {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.width == rhs.width &&
           lhs.height == rhs.height && lhs.depth == rhs.depth;
  }

  inline bool operator==(const size& lhs, const size& rhs)
  {
    return lhs.width == rhs.width && lhs.height == rhs.height;
  }

  inline bool operator!=(const rect& lhs, const rect& rhs)
  {
    return !(lhs == rhs);
  }

  inline bool operator!=(const cube& lhs, const cube& rhs)
  {
    return !(lhs == rhs);
  }

  inline bool operator!=(const size& lhs, const size& rhs)
  {
    return !(lhs == rhs);
  }
} // namespace  mgl::core
