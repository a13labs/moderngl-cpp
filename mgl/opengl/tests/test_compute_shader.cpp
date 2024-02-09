#ifdef MGL_PLATFORM_WINDOWS
#  include "mgl_opengl/context.hpp"
#  include <gtest/gtest.h>

TEST(ComputeShaderTest, ComputeShader)
{
  static mgl::uint8_buffer in = { 1, 2, 3, 4 };
  static mgl::uint8_buffer out = { 0, 0, 0, 0 };

  auto ctx1 = mgl::opengl::create_context(mgl::opengl::context_mode::STANDALONE);
  ASSERT_NE(ctx1, nullptr);
  ASSERT_GE(ctx1->version(), 430);

  auto compute_shader = ctx1->compute_shader(
      R"(
        #version 430

        layout (local_size_x = 1, local_size_y = 1) in;

        layout (std430, binding = 1) buffer Input {
            float v1[4];
        };

        layout (std430, binding = 2) buffer Output {
            float v2[4];
        };

        void main() {
            v2[0] = v1[3];
            v2[1] = v1[2];
            v2[2] = v1[1];
            v2[3] = v1[0];
        }
      )");

  ASSERT_NE(compute_shader, nullptr);

  auto buf1 = ctx1->buffer(in);
  auto buf2 = ctx1->buffer(out);

  buf1->bind_to_storage_buffer(1);
  buf2->bind_to_storage_buffer(2);

  compute_shader->run();

  buf2->read(out);

  ASSERT_EQ(out[0], 4);
  ASSERT_EQ(out[1], 3);
  ASSERT_EQ(out[2], 2);
  ASSERT_EQ(out[3], 1);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#else
int main(int argc, char** argv)
{
  return 0;
}
#endif