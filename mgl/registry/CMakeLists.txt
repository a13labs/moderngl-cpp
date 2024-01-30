project(mgl_registry)

# Add the C source files for the library
file(GLOB_RECURSE LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Create the static library target "mgl_registry"
add_library(mgl_registry STATIC ${LIB_SOURCES})
add_library(mgl::registry-static ALIAS mgl_registry)

set(MGL_REGISTRY_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc" CACHE PATH "Path to mgl::registry include directory")

set (DEFAULT_INCLUDE_DIRS 
    ${MGL_CORE_INCLUDE_DIR}
    ${VENDORS_HEADERS_ONLY_INC_DIR}
    ${GLM_INCLUDE_DIR}
    ${MGL_REGISTRY_INCLUDE_DIR}
)

# Add include directories
target_include_directories(mgl_registry PRIVATE ${DEFAULT_INCLUDE_DIRS})

# Link library to required libraries
target_link_libraries(mgl_registry PRIVATE mgl::core-static)

# compiler and OS definitions
target_compile_definitions(mgl_registry 
    PRIVATE 
    ${MGL_COMPILER_DEFINITION} 
    ${MGL_BUILD_TYPE_DEFINITIONS}
    ${MGL_REGISTRY_DEFINITIONS}
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
        add_executable(mgl_registry_${TEST_NAME} ${FILE_PATH})
        # Link mgl_registry and Check library to the unit test executable
        target_link_libraries(mgl_registry_${TEST_NAME} PRIVATE mgl_registry gtest)
        target_include_directories(mgl_registry_${TEST_NAME} PRIVATE 
                        ${DEFAULT_INCLUDE_DIRS} 
                        ${GTEST_INSTALL_DIR}/include)
        # Add a custom test target to run the unit tests
        add_test(NAME mgl_registry_ctest_${TEST_NAME} COMMAND mgl_registry_${TEST_NAME})
    endforeach()

    add_custom_target(mgl_registry_test_data ALL)
    # Get all tests data available in the test folder and copy it
    file(GLOB_RECURSE GAMELIB_TESTS_DATA "${CMAKE_CURRENT_SOURCE_DIR}/tests/data/*.*")
    set(TESTS_DATA_FOLDER "${CMAKE_CURRENT_BINARY_DIR}")
    foreach(FILE_PATH ${GAMELIB_TESTS_DATA})
        # Get the relative path of the file
        file(RELATIVE_PATH RELATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/tests ${FILE_PATH})
        # Get the destination folder path
        get_filename_component(DESTINATION_FOLDER "${TESTS_DATA_FOLDER}/${RELATIVE_PATH}" DIRECTORY)
        # Create the destination folder
        file(MAKE_DIRECTORY ${DESTINATION_FOLDER})
        # Copy the file to the destination folder
        add_custom_command(
            TARGET mgl_registry_test_data
            POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            ARGS -E copy  ${FILE_PATH} "${DESTINATION_FOLDER}"
        )
    endforeach(FILE_PATH)
endif()