project(compute_shader)

# Add the C source files for the application
file(GLOB_RECURSE APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Create the executable target "window"
add_executable(compute_shader ${APP_SOURCES})

# Add include directories
include_directories(compute_shader PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

# Link gamelib to the application
target_link_libraries(compute_shader PRIVATE mgl::static)
