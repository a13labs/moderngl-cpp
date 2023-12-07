cmake_minimum_required(VERSION 3.12.0)
project(glad)

if (MGL_LINUX_OS)
    add_library(glad-static STATIC src/gl.c src/egl.c)
elseif(MGL_WINDOWS_OS)
    add_library(glad-static STATIC src/gl.c src/wgl.c)
elseif(MGL_OSX_OS)
    add_library(glad-static src/gl.c)
endif()
add_library(glad::static ALIAS glad-static)

target_include_directories(glad-static PRIVATE include)
target_link_libraries(glad-static)



