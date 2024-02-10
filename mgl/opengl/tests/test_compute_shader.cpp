#include "mgl_opengl/context.hpp"
#include <gtest/gtest.h>

TEST(ComputeShaderTest, ComputeShader)
{
  static mgl::float32_buffer in = { 1, 2, 3, 4 };
  static mgl::float32_buffer out = { 0, 0, 0, 0 };

  auto ctx1 = mgl::opengl::create_context(mgl::opengl::context_mode::STANDALONE);
  ASSERT_NE(ctx1, nullptr);
  ASSERT_GE(ctx1->version(), 430);

  auto compute_shader = ctx1->compute_shader(
      R"(
          #version 430

          layout(std430, binding = 0) buffer InputBuffer {
              float input_data[];
          };

          layout(std430, binding = 1) buffer OutputBuffer {
              float output_data[];
          };

          layout (local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

          void main() {
              // Get the global index of the current work item
              uint index = gl_GlobalInvocationID.x;

              // Ensure that the index is within the bounds of the buffer
              if (index < input_data.length()) {
                  // Perform the computation (e.g., multiply by a constant factor)
                  output_data[index] = input_data[index] * 2.0;
              } 
          }
      )");

  ASSERT_NE(compute_shader, nullptr);

  auto buf1 = ctx1->buffer(in);
  auto buf2 = ctx1->buffer(out);

  buf1->bind_to_storage_buffer(0);
  buf2->bind_to_storage_buffer(1);
  compute_shader->run();

  buf2->read(out);

  ASSERT_EQ(out[0], 2);
  ASSERT_EQ(out[1], 4);
  ASSERT_EQ(out[2], 6);
  ASSERT_EQ(out[3], 8);

  compute_shader->release();
  buf1->release();
  buf2->release();
  ctx1->release();
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
