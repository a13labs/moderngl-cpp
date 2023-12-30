cmake_minimum_required(VERSION 3.12.0)
project(mojoAL C)

# Set the C20 standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(mojoal-static STATIC src/mojoal.c)
add_library(MojoAL::static ALIAS mojoal-static)

target_include_directories(mojoal-static PRIVATE ${SDL2_INCLUDE_DIR} inc/AL)
target_link_libraries(mojoal-static SDL2::SDL2-static)
