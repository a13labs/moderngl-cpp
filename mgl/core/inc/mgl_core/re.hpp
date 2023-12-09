#pragma once
#include "builtins.hpp"
#include "string.hpp"

namespace mgl::core
{

  string_list re_split(const string& input, string re);

}