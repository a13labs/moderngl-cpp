project(basic_uniforms_and_attributes)

# Add the C source files for the application
file(GLOB_RECURSE APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Create the executable target "game"
add_executable(basic_uniforms_and_attributes ${APP_SOURCES})

set (DEFAULT_INCLUDE_DIRS 
    ${CMAKE_SOURCE_DIR}/mgl/core/inc
    ${CMAKE_SOURCE_DIR}/mgl/opengl/inc
    ${CMAKE_SOURCE_DIR}/mgl/window/inc
    ${CMAKE_SOURCE_DIR}/vendors/spdlog/inc
    ${CMAKE_SOURCE_DIR}/vendors/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/inc
)

# Add include directories
include_directories(basic_uniforms_and_attributes PRIVATE ${DEFAULT_INCLUDE_DIRS})

# Link gamelib to the application
target_link_libraries(basic_uniforms_and_attributes PRIVATE mgl::mgl_core mgl::mgl_opengl mgl::mgl_window)

# Link statically to the external projects
target_link_libraries(basic_uniforms_and_attributes PRIVATE m)
