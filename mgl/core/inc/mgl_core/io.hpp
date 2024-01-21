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
#pragma once

#include "containers.hpp"
#include "memory.hpp"

#include <filesystem>
#include <fstream>

/**
 * @file io.hpp
 * @brief Contains the definition of the mgl namespace and related types for core input/output functionality.
 */
/**
  * @brief Namespace for core input/output functionality.
  */
namespace mgl
{
  /**
   * @brief Alias for std::filesystem::path.
   */
  using path = std::filesystem::path;

  /**
   * @brief Alias for mgl::list<mgl::path>.
   */
  using path_list = mgl::list<path>;

  /**
   * @brief Alias for std::ifstream.
   */
  using input_file = std::ifstream;

  /**
   * @brief Alias for std::ofstream.
   */
  using output_file = std::ofstream;

  /**
   * @brief Alias for std::ifstream::openmode.
   */
  using openmode = std::ifstream::openmode;

  /**
   * @brief Alias for std::istream.
   */
  using ifsteam = std::istream;

  /**
   * @brief Alias for std::ostream.
   */
  using ofsteam = std::ostream;

  /**
   * @brief Alias for mgl::ref<ifsteam>.
   */
  using ifsteam_ptr = mgl::ref<ifsteam>;

  /**
   * @brief Alias for mgl::ref<ofsteam>.
   */
  using ofsteam_ptr = mgl::ref<ofsteam>;

  /**
   * @brief Constant representing a null path.
   */
  extern const path null_path;

} // namespace  mgl
