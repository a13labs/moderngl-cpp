#include "mgl_core/log.hpp"
#include "mgl_registry/location.hpp"
#include "mgl_registry/locations/local.hpp"
#include "mgl_registry/locations/zip.hpp"
#include "gtest/gtest.h"

TEST(url_test, test_url_parsing)
{
  mgl::registry::url url;

  url.parse("file://test.txt");
  EXPECT_EQ(url.protocol, "file");
  EXPECT_EQ(url.host, "");
  EXPECT_EQ(url.path, "test.txt");
  EXPECT_EQ(url.path.is_relative(), true);
  EXPECT_EQ(url.query, "");

  url.parse("file:///test.txt");
  EXPECT_EQ(url.protocol, "file");
  EXPECT_EQ(url.host, "");
  EXPECT_EQ(url.path, "/test.txt");
  EXPECT_EQ(url.path.is_absolute(), true);
  EXPECT_EQ(url.query, "");

  url.parse("http://localhost/test.txt");
  EXPECT_EQ(url.protocol, "http");
  EXPECT_EQ(url.host, "localhost");
  EXPECT_EQ(url.path, "/test.txt");
  EXPECT_EQ(url.path.is_absolute(), true);
  EXPECT_EQ(url.query, "");

  url.parse("http://localhost/test.txt?query");
  EXPECT_EQ(url.protocol, "http");
  EXPECT_EQ(url.host, "localhost");
  EXPECT_EQ(url.path, "/test.txt");
  EXPECT_EQ(url.path.is_absolute(), true);
  EXPECT_EQ(url.query, "query");
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}