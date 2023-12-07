cmake_minimum_required(VERSION 3.21)
project(duna 
        VERSION 0.0.1
        LANGUAGES CXX
       )

# Enable the FetchContent module
include(FetchContent)

# Build options
option(MGL_BUILD_TESTS "Enable ModernGL-Cpp unit tests" ON)
option(MGL_VENDOR_MOJOAL_BUILD_TESTS "Enable MojoAL unit tests" OFF)
option(MGL_BUILD_DOCS "Build ModernGL-Cpp documentation" OFF)
option(MGL_BUILD_EXAMPLES "Build ModernGL-Cpp documentation" ON)

# Set the C20 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the location for external projects
set(EXTERNAL_PROJECTS_DIR "${CMAKE_CURRENT_BINARY_DIR}/external")

# Set the location for downloaded projects
set(SCRIPTS_DIR "${CMAKE_SOURCE_DIR}/scripts")

# Game data file
set(MGL_DATA_FILE "data.mgz")

# Detect the compiler and set MGL_COMPILER_DEFINITION
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(MGL_COMPILER_DEFINITION "-DMGL_COMPILER_GNU")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(MGL_COMPILER_DEFINITION "-DMGL_COMPILER_MSVC")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(MGL_COMPILER_DEFINITION "-DMGL_COMPILER_CLANG")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(MGL_COMPILER_DEFINITION "-DMGL_COMPILER_INTEL")
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
    set(MGL_LINUX_OS TRUE)
    set(MGL_OS_DEFINITION "-DMGL_OS_LINUX")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(MGL_WINDOWS_OS TRUE)
    set(MGL_OS_DEFINITION "-DMGL_OS_WINDOWS")

    # Setup SDL2
    set(SDL_VERSION "2.28.1")
    set(SDL2_SCRIPT "${SCRIPTS_DIR}/download_sdl2.ps1")

    # Set the SDL2 extraction directory
    set(SDL2_EXTRACT_PATH "/SDL2")
    set(SDL2_DIR "${EXTERNAL_PROJECTS_DIR}/SDL2-${SDL_VERSION}")

    # Add the SDL2 installation directory to the CMAKE_PREFIX_PATH
    list(APPEND CMAKE_PREFIX_PATH "${SDL2_DIR}")

    # Run the PowerShell script during the configure step
    execute_process(
        COMMAND powershell.exe -ExecutionPolicy Bypass -File ${SDL2_SCRIPT} -ExtractPath ${EXTERNAL_PROJECTS_DIR} -SDL2Version ${SDL_VERSION}
        RESULT_VARIABLE SDL2_SCRIPT_RESULT
    )

    if(NOT SDL2_SCRIPT_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to run the PowerShell script to download and extract SDL2.")
    endif()

elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(MGL_OSX_OS TRUE)
    set(MGL_OS_DEFINITION "-DMGL_OS_MACOS")
else()
    message(FATAL_ERROR "Unsupported operating system!")
endif()

if (MGL_BUILD_TESTS)
    # TODO: Setup testing on windows and macOS
    # Add unit tests
    message(STATUS "ModernGL-Cpp tests: ON")
    enable_testing()
    include(Dart)
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

if(MGL_BUILD_TESTS)
    # Set the path where you want to download Google Test
    set(GTEST_INSTALL_DIR "${CMAKE_BINARY_DIR}/googletest" CACHE PATH "Path to Google Test installation directory")

    # Use FetchContent to download Google Test
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.13.0 
    )

    # FetchContent will download Google Test and build it at configure time
    FetchContent_MakeAvailable(googletest)
endif()

# # Include CMakeLists.txt files for the targets
add_subdirectory(vendors)
add_subdirectory(mgl)
if(MGL_BUILD_EXAMPLES) 
    add_subdirectory(examples)
endif()
