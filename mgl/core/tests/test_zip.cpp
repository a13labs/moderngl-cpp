#include "mgl_core/zip.hpp"
#include "gtest/gtest.h"

TEST(mgl_core, zip_test)
{
  mgl::zip_file zip("data/test.zip");

  EXPECT_TRUE(zip.is_valid());

  EXPECT_TRUE(zip.exists("test.txt"));

  mgl::uint8_buffer buffer;
  zip.read("test.txt", buffer);

  EXPECT_TRUE(buffer.size() > 0);
}

TEST(mgl_core, zip_ifstream_test)
{
  mgl::zip_ifstream zip("data/test.zip", "test.txt");

  std::string line;
  std::getline(zip, line);

  EXPECT_EQ(line, "This is a text");

  EXPECT_TRUE(line.length() > 0);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
