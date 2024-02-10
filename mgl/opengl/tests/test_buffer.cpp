#include "mgl_opengl/context.hpp"
#include <gtest/gtest.h>

TEST(BufferTest, CopyTestNoOffset)
{
  auto ctx = mgl::opengl::create_context(mgl::opengl::context_mode::STANDALONE);
  ASSERT_NE(ctx, nullptr);

  mgl::float32_buffer in = { 1, 2, 3, 4 };
  mgl::float32_buffer out = { 0, 0, 0, 0 };

  auto buf1 = ctx->buffer(in);
  auto buf2 = ctx->buffer(4 * sizeof(float));

  // copy all elements from buf1 to buf2
  buf1->copy_to(buf2);
  buf2->read(out);
  ASSERT_EQ(in, out);

  buf1->release();
  buf2->release();
  ctx->release();
}

TEST(BufferTest, CopyTestWithOffset)
{
  auto ctx = mgl::opengl::create_context(mgl::opengl::context_mode::STANDALONE);
  ASSERT_NE(ctx, nullptr);

  mgl::float32_buffer in = { 1, 2, 3, 4, 5, 6, 7, 8 };
  mgl::float32_buffer out = { 0, 0, 0, 0, 0, 0, 0, 0 };
  mgl::float32_buffer expected = { 3, 4, 5, 6, 7, 8, 1, 2 };

  auto buf1 = ctx->buffer(in);
  auto buf2 = ctx->buffer(8 * sizeof(float));

  // copy 2 elements from buf1 to buf2 starting at index 2
  buf1->copy_to(buf2, 2 * sizeof(float), 2 * sizeof(float), 0 * sizeof(float));
  buf1->copy_to(buf2, 2 * sizeof(float), 4 * sizeof(float), 2 * sizeof(float));
  buf1->copy_to(buf2, 2 * sizeof(float), 6 * sizeof(float), 4 * sizeof(float));
  buf1->copy_to(buf2, 2 * sizeof(float), 0 * sizeof(float), 6 * sizeof(float));

  buf2->read(out);
  ASSERT_EQ(expected, out);

  buf1->release();
  buf2->release();
  ctx->release();
}

TEST(BufferTest, NewTest)
{
  auto ctx = mgl::opengl::create_context(mgl::opengl::context_mode::STANDALONE);
  ASSERT_NE(ctx, nullptr);

  mgl::float32_buffer in = { 1, 2, 3, 4 };
  mgl::float32_buffer out = { 0, 0, 0, 0 };

  auto buf = ctx->buffer(in);
  buf->read(out);
  ASSERT_EQ(in, out);

  buf->release();
  ctx->release();
}

TEST(BufferTest, WriteTest)
{
  auto ctx = mgl::opengl::create_context(mgl::opengl::context_mode::STANDALONE);
  ASSERT_NE(ctx, nullptr);

  mgl::float32_buffer in = { 1, 2, 3, 4 };
  mgl::float32_buffer out = { 0, 0, 0, 0 };

  auto buf = ctx->buffer(4 * sizeof(float));

  buf->write(in);
  buf->read(out);
  ASSERT_EQ(in, out);

  buf->release();
  ctx->release();
}

TEST(BufferTest, WriteTestWithOffset)
{
  auto ctx = mgl::opengl::create_context(mgl::opengl::context_mode::STANDALONE);
  ASSERT_NE(ctx, nullptr);

  mgl::float32_buffer in_1 = { 1, 2, 3, 4 };
  mgl::float32_buffer in_2 = { 5, 6, 7, 8 };
  mgl::float32_buffer out = { 0, 0, 0, 0, 0, 0, 0, 0 };
  mgl::float32_buffer expected = { 5, 6, 7, 8, 1, 2, 3, 4 };

  auto buf = ctx->buffer(8 * sizeof(float));

  buf->write(in_1, 4 * sizeof(float));
  buf->write(in_2, 0 * sizeof(float));
  buf->read(out);
  ASSERT_EQ(expected, out);

  buf->release();
  ctx->release();
}

TEST(BufferTest, ReadTest)
{
  auto ctx = mgl::opengl::create_context(mgl::opengl::context_mode::STANDALONE);
  ASSERT_NE(ctx, nullptr);

  mgl::float32_buffer in = { 1, 2, 3, 4 };
  mgl::float32_buffer out = { 0, 0, 0, 0 };

  auto buf = ctx->buffer(in);

  buf->read(out);

  ASSERT_EQ(in, out);

  buf->release();
  ctx->release();
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
