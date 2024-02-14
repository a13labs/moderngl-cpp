project(basic_index_buffer)

# Add the C source files for the application
file(GLOB_RECURSE APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Create the executable target "window"
add_executable(basic_index_buffer ${APP_SOURCES})

# Link gamelib to the application
target_link_libraries(basic_index_buffer PRIVATE mgl::static)

