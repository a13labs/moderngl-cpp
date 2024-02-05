cmake_minimum_required(VERSION 3.12.0)
project(simplelog)

add_library(simplelog_static STATIC src/log.c)
add_library(simplelog::static ALIAS simplelog_static)
target_include_directories(simplelog_static 
  PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
  PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/inc/simplelog
)
target_compile_definitions(
    simplelog_static 
  PRIVATE 
    "-DLOG_USE_COLOR"
)

set(SIMPLELOG_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc" CACHE PATH "Path to spdlog include directory")
