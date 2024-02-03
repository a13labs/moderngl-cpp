project(mgl_registry)

# Add the C source files for the library
file(GLOB_RECURSE LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Create the static library target "mgl_registry"
add_library(mgl_registry_static STATIC ${LIB_SOURCES})
add_library(mgl::registry::static ALIAS mgl_registry_static)

set(MGL_REGISTRY_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc" CACHE PATH "Path to mgl::registry include directory")

# Add include directories
target_include_directories(
    mgl_registry_static 
  PUBLIC
    $<BUILD_INTERFACE:${MGL_REGISTRY_INCLUDE_DIR}>
  PRIVATE 
    ${VENDORS_HEADERS_ONLY_INC_DIR}
    ${CMAKE_BINARY_DIR}/inc        
)

# Link library to required libraries
target_link_libraries(
    mgl_registry_static 
  PRIVATE 
    mgl::core::static
)

# compiler and OS definitions
target_compile_definitions(mgl_registry_static 
    PRIVATE 
    ${MGL_COMPILER_DEFINITION} 
    ${MGL_BUILD_TYPE_DEFINITIONS}
)

if (MGL_BUILD_TESTS)
    find_unit_tests(
      # Required libraries
      mgl::core::static 
      mgl_registry_static 
    )
endif()

install(TARGETS mgl_registry_static
    EXPORT mgl-targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION inc
)