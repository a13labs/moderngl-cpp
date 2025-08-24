project(mgl_platform)

# Add the C source files for the library
file(GLOB_RECURSE LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(APPEND LIB_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/api/metal/wrappers.cpp
)

# Create the static library target "mgl_platform"
add_library(mgl_platform_static STATIC ${LIB_SOURCES})
add_library(mgl::platform::static ALIAS mgl_platform_static)

# Add include directories
target_include_directories(
    mgl_platform_static
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc/public>
  PRIVATE 
    ${CMAKE_CURRENT_BINARY_DIR}/inc
    ${CMAKE_BINARY_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/internal
)

if(MGL_RENDER_API STREQUAL "OpenGL")
    set(LIB_RENDER_OBJECT mgl::opengl::static)
elseif(MGL_RENDER_API STREQUAL "Metal")
    set(LIB_RENDER_OBJECT mgl::metal::static)
else()
    message(FATAL_ERROR "Unknown MGL_RENDER_API: ${MGL_RENDER_API}")
endif()

# Link library to required external libraries
target_link_libraries(
    mgl_platform_static 
  PRIVATE 
    mgl::core::static 
    mgl::registry::static
    ${LIB_RENDER_OBJECT}
)

# Link library to SDL2 dependencies
target_include_directories(
    mgl_platform_static 
  PRIVATE 
    $<TARGET_PROPERTY:SDL2::SDL2-static,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(
    mgl_platform_static 
  PUBLIC
    ${SDL2_LIB_DEPENDS}
  PRIVATE 
    $<TARGET_OBJECTS:SDL2::SDL2-static>
)

# compiler and OS definitions
target_compile_definitions(
    mgl_platform_static 
  PRIVATE 
    ${MGL_COMPILER_DEFINITION} 
    ${MGL_BUILD_TYPE_DEFINITIONS}
)

if (MGL_BUILD_TESTS)
  find_unit_tests(mgl_platform_static)
endif()

# Add the install target
install(TARGETS mgl_platform_static
    EXPORT mgl-targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION inc
)