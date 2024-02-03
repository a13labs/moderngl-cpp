project(mgl_application)

# Add the C source files for the library
file(GLOB_RECURSE LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Create the static library target "mgl_application"
add_library(mgl_application_static STATIC ${LIB_SOURCES})
add_library(mgl::application::static ALIAS mgl_application_static)

# Add include directories
target_include_directories(
    mgl_application_static
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
  PRIVATE 
    ${CMAKE_CURRENT_BINARY_DIR}/inc
    ${CMAKE_BINARY_DIR}/inc        
)

# Link library to required libraries
target_link_libraries(
    mgl_application_static 
  PRIVATE
    mgl::core::static
    mgl::registry::static
    mgl::opengl::static
    mgl::window::static
    mgl::graphics::static
  )

# compiler and OS definitions
target_compile_definitions(
    mgl_application_static 
  PRIVATE 
    ${MGL_COMPILER_DEFINITION} 
    ${MGL_BUILD_TYPE_DEFINITIONS}
    ${MGL_APPLICATION_DEFINITIONS}
)

if (MGL_BUILD_TESTS)
  find_unit_tests(mgl_application_static)
endif()

install(TARGETS mgl_application_static
    EXPORT mgl-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION inc
)
