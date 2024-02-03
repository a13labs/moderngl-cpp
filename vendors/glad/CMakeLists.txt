cmake_minimum_required(VERSION 3.12.0)
project(glad)

if (MGL_LINUX_OS)
    add_library(glad_static STATIC src/gl.c src/egl.c)
elseif(MGL_WINDOWS_OS)
    add_library(glad_static STATIC src/gl.c src/wgl.c)
elseif(MGL_OSX_OS)
    add_library(glad_static src/gl.c)
endif()

add_library(glad::static ALIAS glad_static)

target_include_directories(
    glad_static 
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set(GLAD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE PATH "Path to glad include directory")