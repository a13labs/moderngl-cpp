#ifndef MGL_PLATFORM_MACOS
#  include "mgl_opengl/context.hpp"
#  include <gtest/gtest.h>

TEST(ContextText, StandaloneContext)
{
  static mgl::uint8_buffer in_1 = { 1, 2, 3, 4 };
  static mgl::uint8_buffer in_2 = { 5, 6, 7, 8 };
  static mgl::uint8_buffer out(4);

  auto ctx1 = mgl::opengl::create_context(mgl::opengl::context_mode::STANDALONE);
  ASSERT_NE(ctx1, nullptr);

  auto ctx2 = mgl::opengl::create_context(mgl::opengl::context_mode::STANDALONE);
  ASSERT_NE(ctx2, nullptr);

  ctx1->enter();
  auto buf1 = ctx1->buffer(in_1);

  ctx2->enter();
  auto buf2 = ctx2->buffer(in_2);

  ASSERT_NE(buf1, buf2);
  ASSERT_EQ(buf1->glo(), 1);
  ASSERT_EQ(buf2->glo(), 1);

  ctx1->enter();
  buf1->read(out);
  ASSERT_EQ(in_1, out);

  ctx2->enter();
  buf2->read(out);
  ASSERT_EQ(in_2, out);

  ctx1->release();
  ctx2->release();
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif