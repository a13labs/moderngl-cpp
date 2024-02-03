project(mgl_opengl)

# Add the C source files for the library
file(GLOB_RECURSE LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Create the static library target "mgl_opengl"
add_library(mgl_opengl_static STATIC ${LIB_SOURCES})
add_library(mgl::opengl::static ALIAS mgl_opengl_static)

set(MGL_OPENGL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc" CACHE PATH "Path to mgl::opengl include directory")

# Add include directories
target_include_directories(
    mgl_opengl_static
  PUBLIC
    $<BUILD_INTERFACE:${MGL_OPENGL_INCLUDE_DIR}>
  PRIVATE 
    ${GLAD_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/inc
    ${CMAKE_BINARY_DIR}/inc
)

if(WIN32)
  message(STATUS "OpenGL Platform: Windows")
  set(MGL_OPENGL_PLATFORM_DEFINITION "-DMGL_OPENGL_WGL")
elseif (UNIX)
  if (NOT APPLE)
    set(MGL_OPENGL_PLATFORM_DEFINITION "-DMGL_OPENGL_EGL")
  else()
    message(STATUS "OpenGL Platform: MacOS")
    set(MGL_OPENGL_PLATFORM_DEFINITION "-DMGL_OPENGL_CGL")
  endif()
endif()

# Link library to required libraries
target_link_libraries(
    mgl_opengl_static 
  PRIVATE 
    mgl::core::static 
    $<TARGET_OBJECTS:glad::static>
    ${OPENGL_LIBRARIES}
)

# compiler and OS definitions
target_compile_definitions(
  mgl_opengl_static 
    PRIVATE 
  ${MGL_COMPILER_DEFINITION} 
  ${MGL_BUILD_TYPE_DEFINITIONS}
  ${MGL_OPENGL_PLATFORM_DEFINITION}
)

if (MGL_BUILD_TESTS)
    find_unit_tests(
      mgl::core::static
      glad::static
      mgl_opengl_static
    )
endif()

install(TARGETS mgl_opengl_static
    EXPORT mgl-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION inc
)