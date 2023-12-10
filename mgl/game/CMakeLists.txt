project(mgl_game)

# Configure the config.h file using config.h.in
configure_file(${CMAKE_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/inc/config.h)
                    
# Add the C source files for the library
file(GLOB_RECURSE LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Create the static library target "mgl_game"
add_library(mgl_game STATIC ${LIB_SOURCES})
add_library(mgl::game-static ALIAS mgl_game)

set (DEFAULT_INCLUDE_DIRS 
    ${CMAKE_SOURCE_DIR}/mgl/core/inc
    ${CMAKE_SOURCE_DIR}/mgl/opengl/inc
    ${CMAKE_SOURCE_DIR}/vendors/glm
    ${CMAKE_SOURCE_DIR}/vendors/headers_only
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_BINARY_DIR}/inc
    ${SDL2_INCLUDE_DIR}
)

# Add include directories
include_directories(mgl_game PRIVATE ${DEFAULT_INCLUDE_DIRS})

# Link simplelog library to the unit test executable
target_link_libraries(mgl_game PRIVATE mgl::core-static mgl::opengl-static)

# Link simplelog library to the unit test executable
target_link_libraries(mgl_game PRIVATE SDL2::SDL2-static)

# compiler and OS definitions
target_compile_definitions(mgl_game 
    PRIVATE 
    ${MGL_COMPILER_DEFINITION} 
    ${MGL_OS_DEFINITION} 
    ${MGL_BUILD_TYPE_DEFINITIONS}
    ${MGL_WINDOW_DEFINITIONS}
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
        add_executable(mgl_game_${TEST_NAME} ${FILE_PATH})
        # Link mgl_game and Check library to the unit test executable
        target_link_libraries(mgl_game_${TEST_NAME} PRIVATE mgl_game gtest)
        target_include_directories(mgl_game_${TEST_NAME} PRIVATE 
                        ${DEFAULT_INCLUDE_DIRS} 
                        ${GTEST_INSTALL_DIR}/include)
        # Add a custom test target to run the unit tests
        add_test(NAME mgl_game_ctest_${TEST_NAME} COMMAND mgl_game_${TEST_NAME})
    endforeach()
endif()