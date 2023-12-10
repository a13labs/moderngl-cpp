cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
cmake_policy(VERSION 3.2)

set(GLM_VERSION "0.9.9")
project(glm VERSION ${GLM_VERSION} LANGUAGES CXX)

option(GLM_BUILD_STATIC_LIBS "Buil GLM static library" ON)
option(GLM_BUILD_SHARED_LIBS "Buil GLM shared library" OFF)

add_subdirectory(glm)
add_library(glm::glm ALIAS glm)

set(GLM_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH "Path to glm include directory")