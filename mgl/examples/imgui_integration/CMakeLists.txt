project(imgui_integration)

# Add the C source files for the application
file(GLOB_RECURSE APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Create the executable target "window"
add_executable(imgui_integration ${APP_SOURCES})

set (DEFAULT_INCLUDE_DIRS 
    ${CMAKE_SOURCE_DIR}/inc
    ${MGL_CORE_INCLUDE_DIR}
    ${MGL_OPENGL_INCLUDE_DIR}
    ${MGL_WINDOW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${IMGUI_INCLUDE_DIR}
    ${VENDORS_HEADERS_ONLY_INC_DIR}
)

# Add include directories
include_directories(imgui_integration PRIVATE ${DEFAULT_INCLUDE_DIRS})

# Link gamelib to the application
target_link_libraries(imgui_integration PRIVATE mgl::core-static mgl::opengl-static mgl::window-static ImGUI::static)

# Link statically to the external projects
target_link_libraries(imgui_integration PRIVATE m)
