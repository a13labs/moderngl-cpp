project(mgl_opengl)

# Add the C source files for the library
file(GLOB_RECURSE LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Create the static library target "mgl_opengl"
add_library(mgl_opengl_static STATIC ${LIB_SOURCES})
add_library(mgl::opengl::static ALIAS mgl_opengl_static)

# Add include directories
target_include_directories(
    mgl_opengl_static
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc/public>
  PRIVATE 
    $<TARGET_PROPERTY:glad::static,INTERFACE_INCLUDE_DIRECTORIES>
    ${CMAKE_CURRENT_BINARY_DIR}/inc
    ${CMAKE_BINARY_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/internal
)

if(WIN32)
  message(STATUS "OpenGL Platform: Windows")
  set(MGL_OPENGL_PLATFORM_DEFINITION "-DMGL_OPENGL_WGL")
  target_link_libraries(mgl_opengl_static PUBLIC opengl32)
elseif (UNIX)
  if (NOT APPLE)
    set(MGL_OPENGL_PLATFORM_DEFINITION "-DMGL_OPENGL_EGL")
  else()
    message(STATUS "OpenGL Platform: MacOS")
    set(MGL_OPENGL_PLATFORM_DEFINITION "-DMGL_OPENGL_CGL")
    target_link_libraries(mgl_opengl_static PUBLIC "-framework OpenGL")
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