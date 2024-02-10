#pragma once

#include "mgl_core/containers.hpp"

namespace mgl::opengl
{
  /**
   * @brief The color_mask struct represents the color mask of the OpenGL context.
   * 
   * The color mask is a boolean value that represents whether the red, green, blue, and alpha components of the color buffer are written to.
  */
  struct color_mask
  {
    bool r, g, b, a;

    /**
     * @brief The color_mask constructor creates a new color mask with the specified red, green, blue, and alpha components.
     * 
     * @param r The red component.
     * @param g The green component.
     * @param b The blue component.
     * @param a The alpha component.
     */
    color_mask(bool r, bool g, bool b, bool a)
        : r(r)
        , g(g)
        , b(b)
        , a(a)
    { }

    /**
     * @brief The color_mask constructor creates a new color mask with the specified red, green, and blue components.
     * 
     * @param r The red component.
     * @param g The green component.
     * @param b The blue component.
     */
    color_mask(bool r, bool g, bool b)
        : r(r)
        , g(g)
        , b(b)
        , a(true)
    { }

    /**
     * @brief The color_mask constructor creates a new color mask with the specified red and green components.
     * 
     * @param r The red component.
     * @param g The green component.
     */
    color_mask(bool r, bool g)
        : r(r)
        , g(g)
        , b(true)
        , a(true)
    { }

    /**
     * @brief The color_mask constructor creates a new color mask with the specified red component.
     * 
     * @param r The red component.
     */
    color_mask(bool r)
        : r(r)
        , g(true)
        , b(true)
        , a(true)
    { }

    /**
     * @brief The color_mask constructor creates a new color mask with all components set to true.
     */
    color_mask()
        : r(true)
        , g(true)
        , b(true)
        , a(true)
    { }

    /**
     * @brief The operator== method compares the color mask with another color mask.
     * 
     * @param other The other color mask.
     * @return True if the color mask is equal to the other color mask, false otherwise.
     */
    bool operator==(const color_mask& other) const
    {
      return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    /**
     * @brief The operator!= method compares the color mask with another color mask.
     * 
     * @param other The other color mask.
     * @return True if the color mask is not equal to the other color mask, false otherwise.
     */
    bool operator!=(const color_mask& other) const
    {
      return r != other.r || g != other.g || b != other.b || a != other.a;
    }

    /**
     * @brief The operator! method returns the inverse of the color mask.
     * 
     * @return The inverse of the color mask.
     */
    color_mask operator!() const { return color_mask(!r, !g, !b, !a); }

    /**
     * @brief The operator& method returns the logical AND of the color mask with another color mask.
     * 
     * @param other The other color mask.
     * @return The logical AND of the color mask with the other color mask.
     */
    color_mask operator&(const color_mask& other) const
    {
      return color_mask(r && other.r, g && other.g, b && other.b, a && other.a);
    }

    /**
     * @brief The operator| method returns the logical OR of the color mask with another color mask.
     * 
     * @param other The other color mask.
     * @return The logical OR of the color mask with the other color mask.
     */
    color_mask operator|(const color_mask& other) const
    {
      return color_mask(r || other.r, g || other.g, b || other.b, a || other.a);
    }

    /**
     * @brief The operator~ method returns the bitwise NOT of the color mask.
     * 
     * @return The bitwise NOT of the color mask.
     */
    color_mask operator~() const { return color_mask(!r, !g, !b, !a); }

    /**
     * @brief The operator&= method performs a logical AND of the color mask with another color mask and assigns the result to the color mask.
     * 
     * @param other The other color mask.
     * @return The color mask.
     */
    color_mask& operator&=(const color_mask& other)
    {
      r &= other.r;
      g &= other.g;
      b &= other.b;
      a &= other.a;
      return *this;
    }

    /**
     * @brief The operator|= method performs a logical OR of the color mask with another color mask and assigns the result to the color mask.
     * 
     * @param other The other color mask.
     * @return The color mask.
     */
    color_mask& operator|=(const color_mask& other)
    {
      r |= other.r;
      g |= other.g;
      b |= other.b;
      a |= other.a;
      return *this;
    }
  };

  using color_masks = mgl::list<color_mask>;

} // namespace mgl::opengl
