cmake_minimum_required(VERSION 3.12.0)
project(simplelog)

add_library(simplelog-static STATIC src/log.c)
add_library(simplelog::static ALIAS simplelog-static)
target_include_directories(simplelog-static PRIVATE inc/simplelog)
target_link_libraries(simplelog-static)
target_compile_definitions(simplelog-static PRIVATE "-DLOG_USE_COLOR")

set(SIMPLELOG_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc" CACHE PATH "Path to spdlog include directory")
