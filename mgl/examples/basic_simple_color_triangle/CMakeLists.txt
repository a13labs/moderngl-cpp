project(basic_simple_color_triangle)

# Add the C source files for the application
file(GLOB_RECURSE APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Create the executable target "game"
add_executable(basic_simple_color_triangle ${APP_SOURCES})

set (DEFAULT_INCLUDE_DIRS 
    ${CMAKE_SOURCE_DIR}/inc
    ${MGL_CORE_INCLUDE_DIR}
    ${MGL_OPENGL_INCLUDE_DIR}
    ${MGL_GAME_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${VENDORS_HEADERS_ONLY_INC_DIR}
)

# Add include directories
include_directories(basic_simple_color_triangle PRIVATE ${DEFAULT_INCLUDE_DIRS})

# Link gamelib to the application
target_link_libraries(basic_simple_color_triangle PRIVATE mgl::core-static mgl::opengl-static mgl::game-static)

# Link statically to the external projects
target_link_libraries(basic_simple_color_triangle PRIVATE m)
