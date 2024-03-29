#include "mgl_opengl/context.hpp"
#include <gtest/gtest.h>

TEST(FramebufferTest, GeneralTests)
{
  auto ctx = mgl::opengl::create_context(mgl::opengl::context_mode::STANDALONE);
  ASSERT_NE(ctx, nullptr);

  auto rbo = ctx->renderbuffer(4, 4);
  ASSERT_NE(rbo, nullptr);

  auto dbo = ctx->depth_renderbuffer(4, 4);
  ASSERT_NE(dbo, nullptr);

  auto fbo = ctx->framebuffer({ rbo }, dbo);
  ASSERT_NE(fbo, nullptr);

  ASSERT_NE(fbo->glo(), 0);
  ASSERT_EQ(fbo->width(), 4);
  ASSERT_EQ(fbo->height(), 4);
  ASSERT_EQ(fbo->samples(), 0);
  ASSERT_EQ(fbo->depth_mask(), true);

  fbo->clear(0, 1, 0, 1);

  static mgl::uint8_buffer pixels(4 * 4 * 4);
  fbo->read(pixels, mgl::rect(0, 0, 4, 4), 4);

  for(size_t i = 0; i < pixels.size(); i += 4)
  {
    ASSERT_EQ(pixels[i + 0], 0);
    ASSERT_EQ(pixels[i + 1], 255);
    ASSERT_EQ(pixels[i + 2], 0);
    ASSERT_EQ(pixels[i + 3], 255);
  }

  ASSERT_EQ(fbo->viewport(), mgl::rect(0, 0, 4, 4));

  fbo->set_viewport(mgl::rect(1, 1, 2, 2));
  ASSERT_EQ(fbo->viewport(), mgl::rect(1, 1, 2, 2));

  fbo->clear(1, 0, 0, 1, 1);
  fbo->read(pixels, mgl::rect(0, 0, 4, 4), 4);

  for(size_t i = 0; i < pixels.size(); i += 4)
  {
    if(i >= 4 * 4 * 4)
    {
      ASSERT_EQ(pixels[i + 0], 0);
      ASSERT_EQ(pixels[i + 1], 255);
      ASSERT_EQ(pixels[i + 2], 0);
      ASSERT_EQ(pixels[i + 3], 255);
    }
    else
    {
      ASSERT_EQ(pixels[i + 0], 255);
      ASSERT_EQ(pixels[i + 1], 0);
      ASSERT_EQ(pixels[i + 2], 0);
      ASSERT_EQ(pixels[i + 3], 255);
    }
  }

  fbo->release();
  rbo->release();
  dbo->release();
  ctx->release();
}

TEST(FramebufferTest, HalfFloat)
{
  auto ctx = mgl::opengl::create_context(mgl::opengl::context_mode::STANDALONE);
  ASSERT_NE(ctx, nullptr);

  auto prg = ctx->program({
      R"(
            #version 330

            in vec2 in_vert;

            void main() {
                gl_Position = vec4(in_vert, 0.0, 1.0);
            }
      )",
      R"(
            #version 330

            out vec4 f_color;

            void main() {
                f_color = vec4(0.0, 0.5, 1.0, 1.0);
            }
      )" });

  ASSERT_NE(prg, nullptr);

  static mgl::float32_buffer data = { -1, -1, -1, 1, 1, -1, 1, 1 };
  auto vbo = ctx->buffer(data);
  ASSERT_NE(vbo, nullptr);

  auto vao = ctx->vertex_array(prg, { { vbo, "2f", { "in_vert" } } });
  ASSERT_NE(vao, nullptr);

  auto rbo = ctx->renderbuffer(4, 4, 2);
  ASSERT_NE(rbo, nullptr);

  auto dbo = ctx->depth_renderbuffer(4, 4);
  ASSERT_NE(dbo, nullptr);

  auto fbo = ctx->framebuffer({ rbo }, dbo);
  ASSERT_NE(fbo, nullptr);

  fbo->use();
  fbo->clear();
  vao->render(mgl::opengl::render_mode::TRIANGLE_STRIP);

  static mgl::uint8_buffer pixels(4 * 4 * 2);
  fbo->read(pixels, mgl::rect(0, 0, 4, 4), 2);

  for(size_t i = 0; i < pixels.size(); i += 2)
  {
    ASSERT_EQ(pixels[i + 0], 0);
    ASSERT_EQ(pixels[i + 1], 128);
  }

  fbo->release();
  rbo->release();
  dbo->release();
  vao->release();
  vbo->release();
  prg->release();
  ctx->release();
}

TEST(FramebufferTest, MaskTests)
{
  auto ctx = mgl::opengl::create_context(mgl::opengl::context_mode::STANDALONE);
  ASSERT_NE(ctx, nullptr);

  auto rbo = ctx->renderbuffer(4, 4);
  ASSERT_NE(rbo, nullptr);

  auto fbo = ctx->framebuffer({ rbo }, nullptr);
  ASSERT_NE(fbo, nullptr);

  fbo->use();
  fbo->clear();

  static mgl::uint8_buffer pixels((4 * 4 * 4));
  fbo->read(pixels, mgl::rect(0, 0, 4, 4), 4);

  for(size_t i = 0; i < pixels.size(); i += 4)
  {
    ASSERT_EQ(pixels[i + 0], 0);
    ASSERT_EQ(pixels[i + 1], 0);
    ASSERT_EQ(pixels[i + 2], 0);
    ASSERT_EQ(pixels[i + 3], 0);
  }

  fbo->set_color_mask(mgl::opengl::color_mask(true, false, true, false));
  fbo->clear(0x19 / 255., 0x33 / 255., 0x4c / 255., 0x66 / 255.);

  fbo->read(pixels, mgl::rect(0, 0, 4, 4), 4);

  for(size_t i = 0; i < pixels.size(); i += 4)
  {
    ASSERT_EQ(pixels[i + 0], 0x19);
    ASSERT_EQ(pixels[i + 1], 0x00);
    ASSERT_EQ(pixels[i + 2], 0x4c);
    ASSERT_EQ(pixels[i + 3], 0x00);
  }

  fbo->set_color_mask(mgl::opengl::color_mask(false, true, false, true));
  fbo->clear(0, 0x33 / 255., 0, 0x66 / 255.);

  fbo->read(pixels, mgl::rect(0, 0, 4, 4), 4);

  for(size_t i = 0; i < pixels.size(); i += 4)
  {
    ASSERT_EQ(pixels[i + 0], 0x19);
    ASSERT_EQ(pixels[i + 1], 0x33);
    ASSERT_EQ(pixels[i + 2], 0x4c);
    ASSERT_EQ(pixels[i + 3], 0x66);
  }

  fbo->release();
  rbo->release();
  ctx->release();
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
