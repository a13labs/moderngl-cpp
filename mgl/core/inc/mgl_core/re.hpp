/**
 * @file re.hpp
 * @brief Contains the definition of the re namespace and related types for core regular expression functionality.

*/
#pragma once
#include "string.hpp"

/**
 * \brief The mgl namespace encapsulates the core functionalities of the MGL library, providing a structured and organized way to access these utilities.
 * It includes sub-namespaces such as io (input/output), log (logging) and profiling (profiling and instrumentation). 
 * These namespaces provide specific functionalities for managing file operations, logging messages at different levels, and measuring the execution time of code blocks or functions respectively.
 */
namespace mgl
{
  /**
  * Splits a string into a list of substrings based on a regular expression.
  * 
  * @param input The input string to be split.
  * @param re The regular expression used for splitting the string.
  * @return A list of substrings obtained by splitting the input string.
  */
  string_list re_split(const std::string& input, std::string re);
}