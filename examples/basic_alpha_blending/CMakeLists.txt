project(basic_alpha_blending)

# Add the C source files for the application
file(GLOB_RECURSE APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Create the executable target "window"
add_executable(basic_alpha_blending ${APP_SOURCES})

# Link gamelib to the application
target_link_libraries(basic_alpha_blending PRIVATE mgl::static)
