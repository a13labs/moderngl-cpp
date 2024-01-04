project(basic_graphics_layers)

# Add the C source files for the application
file(GLOB_RECURSE APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Create the executable target "window"
add_executable(basic_graphics_layers ${APP_SOURCES})

set (DEFAULT_INCLUDE_DIRS 
    ${MGL_CORE_INCLUDE_DIR}
    ${MGL_OPENGL_INCLUDE_DIR}
    ${MGL_WINDOW_INCLUDE_DIR}
    ${MGL_ENGINE_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${IMGUI_INCLUDE_DIR}
    ${VENDORS_HEADERS_ONLY_INC_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

# Add include directories
include_directories(basic_graphics_layers PRIVATE ${DEFAULT_INCLUDE_DIRS})

# Link gamelib to the application
target_link_libraries(basic_graphics_layers PRIVATE mgl::graphics-static)
