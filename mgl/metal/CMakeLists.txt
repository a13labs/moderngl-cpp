project(mgl_metal
        LANGUAGES CXX C OBJCXX
       )
# Add the C++ source files for the library
file(GLOB_RECURSE LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.mm)
set_source_files_properties(${LIB_SOURCES} PROPERTIES LANGUAGE OBJCXX)

# Create the static library target "mgl_metal"
add_library(mgl_metal_static STATIC ${LIB_SOURCES})
add_library(mgl::metal::static ALIAS mgl_metal_static)

# Add include directories
target_include_directories(
    mgl_metal_static
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc/public>
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/inc
    ${CMAKE_BINARY_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/internal
)

# Platform-specific settings
if(APPLE)
  message(STATUS "Metal Platform: MacOS")
  set(MGL_METAL_PLATFORM_DEFINITION "-DMGL_METAL_MACOS")
  target_link_libraries(mgl_metal_static PUBLIC "-framework Metal" "-framework Foundation")
else()
  message(FATAL_ERROR "Metal backend is only supported on Apple platforms.")
endif()

# Link library to required libraries
target_link_libraries(
    mgl_metal_static
  PRIVATE
    mgl::core::static
)

# Compiler and OS definitions
target_compile_definitions(
  mgl_metal_static
    PRIVATE
  ${MGL_COMPILER_DEFINITION}
  ${MGL_BUILD_TYPE_DEFINITIONS}
  ${MGL_METAL_PLATFORM_DEFINITION}
)

if (MGL_BUILD_TESTS)
    find_unit_tests(
      mgl::core::static
      mgl_metal_static
    )
endif()

install(TARGETS mgl_metal_static
    EXPORT mgl-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION inc
)
