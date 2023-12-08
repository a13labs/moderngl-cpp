#include "mgl_opengl/buffer_layout.hpp"
#include <gtest/gtest.h>

TEST(BufferLayoutTest, ValidLayout)
{
  // Test a valid buffer layout
  mgl::opengl::buffer_layout layout("3f 2i/v");
  EXPECT_EQ(layout.layout_info().size, 20);
  EXPECT_EQ(layout.layout_info().nodes, 2);
  EXPECT_EQ(layout.layout_info().divisor, 0);
}

TEST(BufferLayoutTest, ValidLayoutDefaultDivision)
{
  // Test a valid buffer layout
  mgl::opengl::buffer_layout layout("3f 2i 4f 10x");
  EXPECT_EQ(layout.layout_info().size, 46);
  EXPECT_EQ(layout.layout_info().nodes, 3);
  EXPECT_EQ(layout.layout_info().divisor, 0);
}

TEST(BufferLayoutTest, InvalidLayout)
{
  // Test an invalid buffer layout
  mgl::opengl::buffer_layout layout("3f 2i/invalid");
  EXPECT_TRUE(layout.layout_info().is_invalid());
}

TEST(BufferLayoutTest, InvalidDivisor)
{
  // Test a buffer layout with default divisor
  mgl::opengl::buffer_layout layout("3f2i");
  EXPECT_TRUE(layout.layout_info().is_invalid());
}

TEST(BufferLayoutTest, PerInstanceDivisor)
{
  // Test a buffer layout with per instance divisor
  mgl::opengl::buffer_layout layout("3f 2i/i");
  EXPECT_EQ(layout.layout_info().size, 20);
  EXPECT_EQ(layout.layout_info().nodes, 2);
  EXPECT_EQ(layout.layout_info().divisor, 1);
}

TEST(BufferLayoutTest, PerRenderDivisor)
{
  // Test a buffer layout with per render divisor
  mgl::opengl::buffer_layout layout("3f 2i/r");
  EXPECT_EQ(layout.layout_info().size, 20);
  EXPECT_EQ(layout.layout_info().nodes, 2);
  EXPECT_EQ(layout.layout_info().divisor, 0x7fffffff);
}

TEST(BufferLayoutTest, PerVertexDivisor)
{
  // Test a buffer layout with per vertex divisor
  mgl::opengl::buffer_layout layout("3f 2i/v");
  EXPECT_EQ(layout.layout_info().size, 20);
  EXPECT_EQ(layout.layout_info().nodes, 2);
  EXPECT_EQ(layout.layout_info().divisor, 0);
}

TEST(BufferLayoutTest, EmptyLayout)
{
  // Test an empty buffer layout
  mgl::opengl::buffer_layout layout("");
  EXPECT_TRUE(layout.layout_info().is_invalid());
}

TEST(BufferLayoutTest, SingleElementLayout)
{
  // Test a buffer layout with a single element
  mgl::opengl::buffer_layout layout("1f");
  EXPECT_EQ(layout.layout_info().size, 4);
  EXPECT_EQ(layout.layout_info().nodes, 1);
  EXPECT_EQ(layout.layout_info().divisor, 0);
}

TEST(BufferLayoutTest, InvalidElementLayout)
{
  // Test a buffer layout with an invalid element
  mgl::opengl::buffer_layout layout("3f 2i/4/invalid");
  EXPECT_TRUE(layout.layout_info().is_invalid());
}

TEST(BufferLayoutTest, NormalizedElementLayout)
{
  // Test a buffer layout with a normalized element and padding
  mgl::opengl::buffer_layout layout("3f1 x");
  EXPECT_EQ(layout.layout_info().size, 4);
  EXPECT_EQ(layout.layout_info().nodes, 1);
  EXPECT_EQ(layout.layout_info().divisor, 0);
}

TEST(BufferLayoutTest, DefaultSizeElementLayout)
{
  // Test a buffer layout with a default size element
  mgl::opengl::buffer_layout layout("3f");
  EXPECT_EQ(layout.layout_info().size, 12);
  EXPECT_EQ(layout.layout_info().nodes, 1);
  EXPECT_EQ(layout.layout_info().divisor, 0);
}

TEST(BufferLayoutTest, DefaultSizeAndCountElementLayout)
{
  // Test a buffer layout with a default size and count element
  mgl::opengl::buffer_layout layout("f");
  EXPECT_EQ(layout.layout_info().size, 4);
  EXPECT_EQ(layout.layout_info().nodes, 1);
  EXPECT_EQ(layout.layout_info().divisor, 0);
}

TEST(BufferLayoutTest, InvalidDataTypeElementLayout)
{
  // Test a buffer layout with an invalid data type element
  mgl::opengl::buffer_layout layout("3z");
  EXPECT_TRUE(layout.layout_info().is_invalid());
}

TEST(BufferLayoutTest, InvalidSizeElementLayout)
{
  // Test a buffer layout with an invalid size element
  mgl::opengl::buffer_layout layout("3fz");
  EXPECT_TRUE(layout.layout_info().is_invalid());
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
