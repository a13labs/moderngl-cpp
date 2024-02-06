cmake_minimum_required(VERSION 3.21)
project(mgl
        VERSION 0.0.1
        LANGUAGES CXX C
       )

# Set scripts directory
set(MGL_SCRIPTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/scripts" CACHE PATH "Path to scripts directory")

# Add paths to modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/" )       

# Enable the FetchContent module
include(FetchContent)

# Include the script for generating shader headers
include(ShadersHeaderGenerator)

# Build options
option(MGL_BUILD_TESTS "Enable ModernGL-Cpp unit tests" ON)
option(MGL_BUILD_DOCS "Build ModernGL-Cpp documentation" OFF)
option(MGL_BUILD_EXAMPLES "Build ModernGL-Cpp documentation" ON)

# Set the C23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the C11 standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Detect the compiler and set MGL_COMPILER_DEFINITION
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(MGL_COMPILER_DEFINITION "-DMGL_COMPILER_GNU")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(MGL_COMPILER_DEFINITION "-DMGL_COMPILER_MSVC")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(MGL_COMPILER_DEFINITION "-DMGL_COMPILER_CLANG")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(MGL_COMPILER_DEFINITION "-DMGL_COMPILER_APPLECLANG")
else()
    message(FATAL_ERROR "Unsupported compiler! ${CMAKE_CXX_COMPILER_ID}" )
endif()

if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
    set(MGL_BUILD_TYPE_DEFINITIONS "-DMGL_RELEASE")
    set(MGL_BUILD_DOCS ON)
else()
    set(MGL_BUILD_TYPE_DEFINITIONS "-DMGL_DEBUG")
endif()

set(MGL_LINUX_OS FALSE)
set(MGL_WINDOWS_OS FALSE)
set(MGL_OSX_OS FALSE)

# Detect the operating system and set MGL_OS_DEFINITION
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Platform: Linux detected")
    set(MGL_LINUX_OS TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Platform: Windows detected")
    set(MGL_WINDOWS_OS TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "Platform: MacOS detected")
    set(MGL_OSX_OS TRUE)
else()
    message(FATAL_ERROR "Unsupported operating system!")
endif()

# Configure the version.hpp file using version.hpp
configure_file(version.hpp.in ${CMAKE_BINARY_DIR}/inc/version.hpp)
                    
if (MGL_BUILD_TESTS)
    include(UnitTestConfig)
    message(STATUS "ModernGL-Cpp tests: ON")
    enable_testing()
else()
    message(STATUS "ModernGL-Cpp tests: OFF")
endif()

if (MGL_BUILD_DOCS)
    message(STATUS "ModernGL-Cpp documentation: ON")
    find_package(Doxygen REQUIRED)
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        # set input and output files
        set(DOXYGEN_IN "${CMAKE_SOURCE_DIR}/doc/Doxyfile.in")
        set(DOXYGEN_OUT "${CMAKE_BINARY_DIR}/Doxyfile")

        # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build started")

        # note the option ALL which allows to build the docs together with the application
        add_custom_target(gamelib_documentation ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM 
            )
    else (DOXYGEN_FOUND)
        message("Doxygen need to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)
else()
    message(STATUS "ModernGL-Cpp documentation: OFF")
endif()

# # Include CMakeLists.txt files for the targets
add_subdirectory(vendors)
add_subdirectory(mgl)

add_library(mgl_static STATIC 
    $<TARGET_OBJECTS:mgl::core::static>
    $<TARGET_OBJECTS:mgl::registry::static>
    $<TARGET_OBJECTS:mgl::opengl::static>
    $<TARGET_OBJECTS:mgl::platform::static>
    $<TARGET_OBJECTS:mgl::graphics::static>
    $<TARGET_OBJECTS:mgl::application::static>
)

add_library(mgl::static ALIAS mgl_static)

# Set the compile definitions
target_link_libraries(mgl_static
  PUBLIC
    mgl::core::static
    mgl::registry::static
    mgl::opengl::static
    mgl::platform::static
    mgl::graphics::static
    mgl::application::static
)

# Build the examples
if(MGL_BUILD_EXAMPLES) 
    add_subdirectory(examples)
endif()

install(TARGETS mgl_static
    EXPORT mgl-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION inc
)

install(EXPORT mgl-targets
    FILE mgl-targets.cmake
    NAMESPACE mgl::
    DESTINATION lib/cmake/mgl
)

# Debug CMake Vars, uncomment to show CMake vars
# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
