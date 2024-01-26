#include "mgl_core/io.hpp"
#include "gtest/gtest.h"

mgl::uint8_buffer s_uint8_buffer = { 0x00, 0x01, 0x02, 0x03, 0x04 };
mgl::uint16_buffer s_uint16_buffer = { 0, 1, 2, 3, 4 };
mgl::uint32_buffer s_uint32_buffer = { 0, 1, 2, 3, 4 };
mgl::uint64_buffer s_uint64_buffer = { 0, 1, 2, 3, 4 };
mgl::int8_buffer s_int8_buffer = { 0, 1, 2, 3, 4 };
mgl::int16_buffer s_int16_buffer = { 0, 1, 2, 3, 4 };
mgl::int32_buffer s_int32_buffer = { 0, 1, 2, 3, 4 };
mgl::int64_buffer s_int64_buffer = { 0, 1, 2, 3, 4 };
mgl::float32_buffer s_float32_buffer = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f };
mgl::float64_buffer s_float64_buffer = { 0.0, 1.0, 2.0, 3.0, 4.0 };

TEST(mgl_core, io_write_read_test)
{

  mgl::path path = "test.txt";
  mgl::uint8_buffer uint8_buffer(s_uint8_buffer.size());
  mgl::uint16_buffer uint16_buffer(s_uint16_buffer.size());
  mgl::uint32_buffer uint32_buffer(s_uint32_buffer.size());
  mgl::uint64_buffer uint64_buffer(s_uint64_buffer.size());
  mgl::int8_buffer int8_buffer(s_int8_buffer.size());
  mgl::int16_buffer int16_buffer(s_int16_buffer.size());
  mgl::int32_buffer int32_buffer(s_int32_buffer.size());
  mgl::int64_buffer int64_buffer(s_int64_buffer.size());
  mgl::float32_buffer float32_buffer(s_float32_buffer.size());
  mgl::float64_buffer float64_buffer(s_float64_buffer.size());

  auto out = mgl::open_write(path);
  mgl::write_string(out, "This is a text");
  mgl::write_uint8(out, 0);
  mgl::write_uint16(out, 1);
  mgl::write_uint32(out, 2);
  mgl::write_uint64(out, 3);
  mgl::write_int8(out, 4);
  mgl::write_int16(out, 5);
  mgl::write_int32(out, 6);
  mgl::write_int64(out, 7);
  mgl::write_float32(out, 8.0f);
  mgl::write_float64(out, 9.0);
  mgl::write_bool(out, true);
  mgl::write_bool(out, false);
  mgl::write_uint8_buffer(out, s_uint8_buffer);
  mgl::write_uint16_buffer(out, s_uint16_buffer);
  mgl::write_uint32_buffer(out, s_uint32_buffer);
  mgl::write_uint64_buffer(out, s_uint64_buffer);
  mgl::write_int8_buffer(out, s_int8_buffer);
  mgl::write_int16_buffer(out, s_int16_buffer);
  mgl::write_int32_buffer(out, s_int32_buffer);
  mgl::write_int64_buffer(out, s_int64_buffer);
  mgl::write_float32_buffer(out, s_float32_buffer);
  mgl::write_float64_buffer(out, s_float64_buffer);
  out->close();

  auto in = mgl::open_read(path);
  auto str = mgl::read_string(in);
  auto uint8 = mgl::read_uint8(in);
  auto uint16 = mgl::read_uint16(in);
  auto uint32 = mgl::read_uint32(in);
  auto uint64 = mgl::read_uint64(in);
  auto int8 = mgl::read_int8(in);
  auto int16 = mgl::read_int16(in);
  auto int32 = mgl::read_int32(in);
  auto int64 = mgl::read_int64(in);
  auto float32 = mgl::read_float32(in);
  auto float64 = mgl::read_float64(in);
  auto bool1 = mgl::read_bool(in);
  auto bool2 = mgl::read_bool(in);
  mgl::read_uint8_buffer(in, uint8_buffer);
  mgl::read_uint16_buffer(in, uint16_buffer);
  mgl::read_uint32_buffer(in, uint32_buffer);
  mgl::read_uint64_buffer(in, uint64_buffer);
  mgl::read_int8_buffer(in, int8_buffer);
  mgl::read_int16_buffer(in, int16_buffer);
  mgl::read_int32_buffer(in, int32_buffer);
  mgl::read_int64_buffer(in, int64_buffer);
  mgl::read_float32_buffer(in, float32_buffer);
  mgl::read_float64_buffer(in, float64_buffer);
  in->close();

  EXPECT_EQ(str, "This is a text");
  EXPECT_EQ(uint8, 0);
  EXPECT_EQ(uint16, 1);
  EXPECT_EQ(uint32, 2);
  EXPECT_EQ(uint64, 3);
  EXPECT_EQ(int8, 4);
  EXPECT_EQ(int16, 5);
  EXPECT_EQ(int32, 6);
  EXPECT_EQ(int64, 7);
  EXPECT_EQ(float32, 8.0f);
  EXPECT_EQ(float64, 9.0);
  EXPECT_EQ(bool1, true);
  EXPECT_EQ(bool2, false);
  EXPECT_EQ(uint8_buffer, s_uint8_buffer);
  EXPECT_EQ(uint16_buffer, s_uint16_buffer);
  EXPECT_EQ(uint32_buffer, s_uint32_buffer);
  EXPECT_EQ(uint64_buffer, s_uint64_buffer);
  EXPECT_EQ(int8_buffer, s_int8_buffer);
  EXPECT_EQ(int16_buffer, s_int16_buffer);
  EXPECT_EQ(int32_buffer, s_int32_buffer);
  EXPECT_EQ(int64_buffer, s_int64_buffer);
  EXPECT_EQ(float32_buffer, s_float32_buffer);
  EXPECT_EQ(float64_buffer, s_float64_buffer);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}