project(basic_uniforms_and_attributes)

# Add the C source files for the application
file(GLOB_RECURSE APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Create the executable target "window"
add_executable(basic_uniforms_and_attributes ${APP_SOURCES})

set (DEFAULT_INCLUDE_DIRS 
    ${MGL_CORE_INCLUDE_DIR}
    ${MGL_OPENGL_INCLUDE_DIR}
    ${MGL_WINDOW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${VENDORS_HEADERS_ONLY_INC_DIR}
)

# Add include directories
include_directories(basic_uniforms_and_attributes PRIVATE ${DEFAULT_INCLUDE_DIRS})

# Link gamelib to the application
target_link_libraries(basic_uniforms_and_attributes PRIVATE mgl::static)
