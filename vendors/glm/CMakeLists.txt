cmake_minimum_required(VERSION 3.12.0)

set(GLM_VERSION "0.9.9")
project(glm VERSION ${GLM_VERSION} LANGUAGES CXX)

option(GLM_BUILD_STATIC_LIBS "Buil GLM static library" ON)
option(GLM_BUILD_SHARED_LIBS "Buil GLM shared library" OFF)

add_subdirectory(glm)
add_library(glm::static ALIAS glm)
target_include_directories(
    glm 
  INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

install(TARGETS glm
    EXPORT mgl-targets
    ARCHIVE DESTINATION lib
)