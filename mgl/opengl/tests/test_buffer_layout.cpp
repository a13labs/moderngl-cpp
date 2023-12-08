#include "mgl_opengl/buffer_layout.hpp"
#include <gtest/gtest.h>

#include "glad/gl.h"

TEST(BufferLayoutTest, ValidLayout)
{
  // Test a valid buffer layout
  mgl::opengl::buffer_layout layout("3f 2i/v");
  EXPECT_EQ(layout.stride(), 20);
  EXPECT_EQ(layout.size(), 2);
  EXPECT_EQ(layout.divisor(), 0);

  EXPECT_EQ(layout[0].size, 12);
  EXPECT_EQ(layout[0].count, 3);
  EXPECT_EQ(layout[0].offset, 0);
  EXPECT_EQ(layout[0].type, GL_FLOAT);

  EXPECT_EQ(layout[1].size, 8);
  EXPECT_EQ(layout[1].count, 2);
  EXPECT_EQ(layout[1].offset, 12);
  EXPECT_EQ(layout[1].type, GL_INT);
}

TEST(BufferLayoutTest, ValidLayoutDefaultDivision)
{
  // Test a valid buffer layout
  mgl::opengl::buffer_layout layout("3f 2i 4f 10x 4f");
  EXPECT_EQ(layout.stride(), 62);
  EXPECT_EQ(layout.size(), 4);
  EXPECT_EQ(layout.divisor(), 0);

  EXPECT_EQ(layout[0].size, 12);
  EXPECT_EQ(layout[0].count, 3);
  EXPECT_EQ(layout[0].offset, 0);
  EXPECT_EQ(layout[0].type, GL_FLOAT);

  EXPECT_EQ(layout[1].size, 8);
  EXPECT_EQ(layout[1].count, 2);
  EXPECT_EQ(layout[1].offset, 12);
  EXPECT_EQ(layout[1].type, GL_INT);

  EXPECT_EQ(layout[2].size, 16);
  EXPECT_EQ(layout[2].count, 4);
  EXPECT_EQ(layout[2].offset, 20);
  EXPECT_EQ(layout[2].type, GL_FLOAT);

  EXPECT_EQ(layout[3].size, 16);
  EXPECT_EQ(layout[3].count, 4);
  EXPECT_EQ(layout[3].offset, 46);
  EXPECT_EQ(layout[3].type, GL_FLOAT);
}

TEST(BufferLayoutTest, InvalidLayout)
{
  // Test an invalid buffer layout
  mgl::opengl::buffer_layout layout("3f 2i/invalid");
  EXPECT_TRUE(layout.is_invalid());
}

TEST(BufferLayoutTest, InvalidDivisor)
{
  // Test a buffer layout with default divisor
  mgl::opengl::buffer_layout layout("3f2i");
  EXPECT_TRUE(layout.is_invalid());
}

TEST(BufferLayoutTest, PerInstanceDivisor)
{
  // Test a buffer layout with per instance divisor
  mgl::opengl::buffer_layout layout("3f 2i/i");
  EXPECT_EQ(layout.stride(), 20);
  EXPECT_EQ(layout.size(), 2);
  EXPECT_EQ(layout.divisor(), 1);
}

TEST(BufferLayoutTest, PerRenderDivisor)
{
  // Test a buffer layout with per render divisor
  mgl::opengl::buffer_layout layout("3f 2i/r");
  EXPECT_EQ(layout.stride(), 20);
  EXPECT_EQ(layout.size(), 2);
  EXPECT_EQ(layout.divisor(), 0x7fffffff);
}

TEST(BufferLayoutTest, PerVertexDivisor)
{
  // Test a buffer layout with per vertex divisor
  mgl::opengl::buffer_layout layout("3f 2i/v");
  EXPECT_EQ(layout.stride(), 20);
  EXPECT_EQ(layout.size(), 2);
  EXPECT_EQ(layout.divisor(), 0);
}

TEST(BufferLayoutTest, EmptyLayout)
{
  // Test an empty buffer layout
  mgl::opengl::buffer_layout layout("");
  EXPECT_TRUE(layout.is_invalid());
}

TEST(BufferLayoutTest, SingleElementLayout)
{
  // Test a buffer layout with a single element
  mgl::opengl::buffer_layout layout("1f");
  EXPECT_EQ(layout.stride(), 4);
  EXPECT_EQ(layout.size(), 1);
  EXPECT_EQ(layout.divisor(), 0);
}

TEST(BufferLayoutTest, InvalidElementLayout)
{
  // Test a buffer layout with an invalid element
  mgl::opengl::buffer_layout layout("3f 2i/4/invalid");
  EXPECT_TRUE(layout.is_invalid());
}

TEST(BufferLayoutTest, NormalizedElementLayout)
{
  // Test a buffer layout with a normalized element and padding
  mgl::opengl::buffer_layout layout("3f1 x");
  EXPECT_EQ(layout.stride(), 4);
  EXPECT_EQ(layout.size(), 1);
  EXPECT_EQ(layout.divisor(), 0);
}

TEST(BufferLayoutTest, DefaultSizeElementLayout)
{
  // Test a buffer layout with a default size element
  mgl::opengl::buffer_layout layout("3f");
  EXPECT_EQ(layout.stride(), 12);
  EXPECT_EQ(layout.size(), 1);
  EXPECT_EQ(layout.divisor(), 0);
}

TEST(BufferLayoutTest, DefaultSizeAndCountElementLayout)
{
  // Test a buffer layout with a default size and count element
  mgl::opengl::buffer_layout layout("f");
  EXPECT_EQ(layout.stride(), 4);
  EXPECT_EQ(layout.size(), 1);
  EXPECT_EQ(layout.divisor(), 0);
}

TEST(BufferLayoutTest, InvalidDataTypeElementLayout)
{
  // Test a buffer layout with an invalid data type element
  mgl::opengl::buffer_layout layout("3z");
  EXPECT_TRUE(layout.is_invalid());
}

TEST(BufferLayoutTest, InvalidSizeElementLayout)
{
  // Test a buffer layout with an invalid size element
  mgl::opengl::buffer_layout layout("3fz");
  EXPECT_TRUE(layout.is_invalid());
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
