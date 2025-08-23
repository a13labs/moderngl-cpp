project(mgl_graphics)

# Add the C source files for the library
file(GLOB_RECURSE LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Generate the shader headers
generate_shader_headers()

# Create the static library target "mgl_graphics"
add_library(mgl_graphics_static STATIC ${LIB_SOURCES})
add_library(mgl::graphics::static ALIAS mgl_graphics_static)

# Add include directories
target_include_directories(
    mgl_graphics_static 
  PUBLIC
    $<TARGET_PROPERTY:imgui::static,INTERFACE_INCLUDE_DIRECTORIES>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
  PRIVATE 
    ${CMAKE_CURRENT_BINARY_DIR}/inc
    ${CMAKE_BINARY_DIR}/inc
)

if(MGL_RENDER_API STREQUAL "OpenGL")
    set(LIB_RENDER_OBJECT $<TARGET_OBJECTS:mgl::opengl::static>)
elseif(MGL_RENDER_API STREQUAL "Metal")
    set(LIB_RENDER_OBJECT $<TARGET_OBJECTS:mgl::metal::static>)
else()
    message(FATAL_ERROR "Unknown MGL_RENDER_API: ${MGL_RENDER_API}")
endif()

# Link library to required libraries
target_link_libraries(
    mgl_graphics_static 
  PUBLIC
    $<TARGET_OBJECTS:imgui::static>
  PRIVATE
    mgl::core::static 
    mgl::registry::static
    ${LIB_RENDER_OBJECT}
    mgl::platform::static
)

# compiler and OS definitions
target_compile_definitions(mgl_graphics_static 
    PRIVATE 
    ${MGL_COMPILER_DEFINITION} 
    ${MGL_BUILD_TYPE_DEFINITIONS}
)

if (MGL_BUILD_TESTS)
  find_unit_tests(mgl_platform_static)
endif()

install(TARGETS mgl_graphics_static
    EXPORT mgl-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION inc
)
