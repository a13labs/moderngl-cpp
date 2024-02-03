project(basic_simple_color_triangle)

# Add the C source files for the application
file(GLOB_RECURSE APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Create the executable target "window"
add_executable(basic_simple_color_triangle ${APP_SOURCES})

# Link gamelib to the application
target_link_libraries(basic_simple_color_triangle PRIVATE mgl::static)
