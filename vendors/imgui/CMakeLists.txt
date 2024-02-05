cmake_minimum_required(VERSION 3.12.0)
project(imgui)

add_library(
    imgui_static 
  STATIC 
    src/imgui.cpp
    src/imgui_draw.cpp
    src/imgui_tables.cpp
    src/imgui_widgets.cpp
)
add_library(imgui::static ALIAS imgui_static)

target_include_directories(
    imgui_static 
  PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
  PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/imgui
)

install(TARGETS imgui_static
    EXPORT mgl-targets
    ARCHIVE DESTINATION lib
)