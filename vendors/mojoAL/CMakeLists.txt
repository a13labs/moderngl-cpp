cmake_minimum_required(VERSION 3.12.0)
project(mojoAL)

add_library(mojoal-static STATIC src/mojoal.c)
target_include_directories(mojoal-static PRIVATE ${SDL2_INCLUDE_DIR} inc/AL)
target_link_libraries(mojoal-static SDL2::SDL2-static )

if (MGL_BUILD_TESTS)
    macro(add_test_executable _NAME)
        add_executable(${_NAME} tests/${_NAME}.c)
        target_link_libraries(${_NAME} mojoal-static)
        target_include_directories(${_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/inc")
        target_include_directories(${_NAME} PRIVATE ${SDL2_INCLUDE_DIR})
        target_link_libraries(${_NAME} SDL2::SDL2-static)
    endmacro()

    add_test_executable(testopenalinfo)
    add_test_executable(testqueueing)
    add_test_executable(testcapture)
    add_test_executable(testposition)
endif()

