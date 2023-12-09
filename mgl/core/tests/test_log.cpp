/*
   Copyright 2022 Alexandre Pires (c.alexandre.pires@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include "mgl_core/log.hpp"
#include "gtest/gtest.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

TEST(mgl_core, log_test)
{
  ::mgl::log::init("test.log");
  MGL_CORE_TRACE("Test MGL_CORE_TRACE");
  MGL_CORE_INFO("Test MGL_CORE_INFO");
  MGL_CORE_WARN("Test MGL_CORE_WARN");
  MGL_CORE_ERROR("Test MGL_CORE_ERROR");
  MGL_CORE_CRITICAL("Test MGL_CORE_CRITICAL");

  std::ifstream file("test.log");
  if(!file.is_open())
  {
    std::cerr << "Error opening file: test.log" << std::endl;
    ::testing::AssertionResult(false);
    return;
  }

  std::regex re_expr(R"(\[.*\] \[.*\] mgl_core: (Test MGL_CORE_.+))");
  std::string line;

  while(std::getline(file, line))
  {
    std::smatch match;
    if(std::regex_search(line, match, re_expr))
    {
      std::string mglCoreString = match[1].str();
      EXPECT_TRUE(mglCoreString.length() > 0);
    }
  }

  EXPECT_TRUE(true);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
