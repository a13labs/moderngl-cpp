cmake_minimum_required(VERSION 3.12.0)
project(mojoAL C)

# Set the C20 standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(mojoal-static STATIC src/mojoal.c)
add_library(MojoAL::static ALIAS mojoal-static)

target_include_directories(mojoal-static PRIVATE ${SDL2_INCLUDE_DIR} inc/AL)
target_link_libraries(mojoal-static SDL2::SDL2-static)

if (MGL_BUILD_TESTS AND NOT WIN32)

    # Get all tests availables in the test folder and add the test
    file(GLOB GAMELIB_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_*.c")
    foreach(FILE_PATH ${GAMELIB_TESTS})
        # Get the filename without extension using get_filename_component
        get_filename_component(TEST_NAME ${FILE_PATH} NAME_WE)
        # Perform actions for each file, for example, displaying the file path and filename without extension
        message(STATUS "MojoAL Test found: ${TEST_NAME}")
        # Create the unit test executable
        add_executable(mojoal_${TEST_NAME} ${FILE_PATH})
        # Link mgl_opengl and Check library to the unit test executable
        target_link_libraries(mojoal_${TEST_NAME} PRIVATE mojoal-static SDL2::SDL2-static)
        target_include_directories(mojoal_${TEST_NAME} PRIVATE 
                       "${CMAKE_CURRENT_SOURCE_DIR}/inc"
                       ${SDL2_INCLUDE_DIR})
        # Add a custom test target to run the unit tests
        add_test(NAME mojoal_ctest_${TEST_NAME} COMMAND mojoal_${TEST_NAME})
    endforeach()
endif()

