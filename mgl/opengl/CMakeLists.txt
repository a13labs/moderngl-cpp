project(mgl_opengl)

# Configure the config.h file using config.h.in
configure_file(${CMAKE_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/inc/config.h)
                    
# Add the C source files for the library
file(GLOB_RECURSE LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Create the static library target "mgl_opengl"
add_library(mgl_opengl STATIC ${LIB_SOURCES})
add_library(mgl::opengl-static ALIAS mgl_opengl)

set (DEFAULT_INCLUDE_DIRS 
    ${CMAKE_SOURCE_DIR}/mgl/core/inc
    ${CMAKE_SOURCE_DIR}/vendors/glm
    ${CMAKE_SOURCE_DIR}/vendors/glad/include
    ${CMAKE_SOURCE_DIR}/vendors/headers_only
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_BINARY_DIR}/inc
)

# Add include directories
include_directories(mgl_opengl PRIVATE ${DEFAULT_INCLUDE_DIRS})

# Link simplelog library to the unit test executable
target_link_libraries(mgl_opengl PRIVATE mgl::core-static glm::glm_static glad::static)

# Find OpenGL package
find_package(OpenGL REQUIRED)
include_directories(mgl_opengl PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(mgl_opengl PRIVATE ${OPENGL_LIBRARIES})

if(WIN32)
    message(STATUS "OpenGL Platform: Windows")
    set(MGL_OPENGL_PLATFORM_DEFINITION "-DMGL_OPENGL_WGL")
elseif (UNIX)
    if (OpenGL_EGL_FOUND)
        message(STATUS "OpenGL Platform: Linux (EGL)")
        set(MGL_OPENGL_PLATFORM_DEFINITION "-DMGL_OPENGL_EGL")
    else()
        message(FATAL_ERROR "OpenGL EGL not found. Please install it using your package manager.")
    endif()
endif()

# compiler and OS definitions
target_compile_definitions(mgl_opengl 
    PRIVATE 
    ${MGL_COMPILER_DEFINITION} 
    ${MGL_OS_DEFINITION} 
    ${MGL_BUILD_TYPE_DEFINITIONS}
    ${MGL_OPENGL_PLATFORM_DEFINITION}
)

if (MGL_BUILD_TESTS)
    # Get all tests availables in the test folder and add the test
    file(GLOB GAMELIB_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_*.cpp")
    foreach(FILE_PATH ${GAMELIB_TESTS})
        # Get the filename without extension using get_filename_component
        get_filename_component(TEST_NAME ${FILE_PATH} NAME_WE)
        # Perform actions for each file, for example, displaying the file path and filename without extension
        message(STATUS "GameLib Test found: ${TEST_NAME}")
        # Create the unit test executable
        add_executable(mgl_opengl_${TEST_NAME} ${FILE_PATH})
        # Link mgl_opengl and Check library to the unit test executable
        target_link_libraries(mgl_opengl_${TEST_NAME} PRIVATE mgl_opengl gtest)
        target_include_directories(mgl_opengl_${TEST_NAME} PRIVATE 
                        ${DEFAULT_INCLUDE_DIRS} 
                        ${GTEST_INSTALL_DIR}/include)
        # Add a custom test target to run the unit tests
        add_test(NAME mgl_opengl_ctest_${TEST_NAME} COMMAND mgl_opengl_${TEST_NAME})
    endforeach()
endif()