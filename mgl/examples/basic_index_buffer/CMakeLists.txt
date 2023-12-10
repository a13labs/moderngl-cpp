project(basic_index_buffer)

# Add the C source files for the application
file(GLOB_RECURSE APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Create the executable target "game"
add_executable(basic_index_buffer ${APP_SOURCES})

set (DEFAULT_INCLUDE_DIRS 
    ${CMAKE_SOURCE_DIR}/mgl/core/inc
    ${CMAKE_SOURCE_DIR}/mgl/opengl/inc
    ${CMAKE_SOURCE_DIR}/mgl/window/inc
    ${CMAKE_SOURCE_DIR}/vendors/glm
    ${CMAKE_SOURCE_DIR}/vendors/headers_only
    ${CMAKE_SOURCE_DIR}/inc
)

# Add include directories
include_directories(basic_index_buffer PRIVATE ${DEFAULT_INCLUDE_DIRS})

# Link gamelib to the application
target_link_libraries(basic_index_buffer PRIVATE mgl::core-static mgl::opengl-static mgl::window-static)

# Link statically to the external projects
target_link_libraries(basic_index_buffer PRIVATE m)
