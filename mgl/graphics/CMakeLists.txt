project(mgl_graphics)

# Include the script for generating shader headers
include(ShadersHeaderGenerator)

# Add the C source files for the library
file(GLOB_RECURSE LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Generate the shader headers
generate_shader_headers()

# Create the static library target "mgl_graphics"
add_library(mgl_graphics STATIC ${LIB_SOURCES})
add_library(mgl::graphics-static ALIAS mgl_graphics)

set(MGL_GRAPHICS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc" CACHE PATH "Path to mgl::graphics include directory")

set (DEFAULT_INCLUDE_DIRS 
    ${MGL_CORE_INCLUDE_DIR}
    ${MGL_OPENGL_INCLUDE_DIR}
    ${MGL_WINDOW_INCLUDE_DIR}
    ${MGL_REGISTRY_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${VENDORS_HEADERS_ONLY_INC_DIR}
    ${SDL2_INCLUDE_DIR}
    ${IMGUI_INCLUDE_DIR}
    ${MGL_GRAPHICS_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/inc
)

# Add include directories
target_include_directories(mgl_graphics PRIVATE ${DEFAULT_INCLUDE_DIRS})

# Link library to required libraries
target_link_libraries(mgl_graphics PRIVATE mgl::window-static mgl::registry-static)

# compiler and OS definitions
target_compile_definitions(mgl_graphics 
    PRIVATE 
    ${MGL_COMPILER_DEFINITION} 
    ${MGL_BUILD_TYPE_DEFINITIONS}
    ${MGL_ENGINE_DEFINITIONS}
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
        add_executable(mgl_graphics_${TEST_NAME} ${FILE_PATH})
        # Link mgl_graphics and Check library to the unit test executable
        target_link_libraries(mgl_graphics_${TEST_NAME} PRIVATE mgl_graphics gtest)
        target_include_directories(mgl_graphics_${TEST_NAME} PRIVATE 
                        ${DEFAULT_INCLUDE_DIRS} 
                        ${GTEST_INSTALL_DIR}/include)
        # Add a custom test target to run the unit tests
        add_test(NAME mgl_graphics_ctest_${TEST_NAME} COMMAND mgl_graphics_${TEST_NAME})
    endforeach()
endif()