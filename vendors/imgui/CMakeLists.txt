cmake_minimum_required(VERSION 3.12.0)
project(ImGUI)

add_library(imgui STATIC 
    src/imgui.cpp
    src/imgui_draw.cpp
    src/imgui_tables.cpp
    src/imgui_widgets.cpp
)
add_library(imgui::static ALIAS imgui)

target_include_directories(imgui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc/imgui)
target_link_libraries(imgui)

set(IMGUI_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc" CACHE PATH "Path to ImGui include directory")

install(TARGETS imgui
    EXPORT mgl-targets
    ARCHIVE DESTINATION lib
)